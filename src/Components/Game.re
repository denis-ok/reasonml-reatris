module RR = ReasonReact;
module Func = Functions;
module Document = Webapi.Dom.Document;
module Const = Constants;
open Types;

[@bs.val] external document: Dom.document = "document";

[%bs.raw {|require('./game.css')|}];

let initGridState = Func.genInitGridState(~gridWidth=10, ~gridHeight=22);

let initGlobalState: globalState = {
  gridState: initGridState,
  nextBlock: Blocks.getRandomBlock(),
  stats: Const.initStats,
  gameOver: false,
};

type state = globalState;

let getNextScreen = currentScreen => {
  switch (currentScreen) {
  | Title => Counter
  | Counter => Game
  | Game => Gameover
  | Gameover => Counter
  };
};

let initTimerIds = {
  tick: ref(None),
  countdown: ref(None),
  moveLeft: ref(None),
  moveRight: ref(None),
  rotate: ref(None),
};

[@react.component]
let make = () => {
  let (timers: timerIds, _setTimer) = React.useState(() => initTimerIds);
  let (screen, setScreen) = React.useState(() => Title);
  let (countdownCounter, setCountdownCounter) = React.useState(() => 0);
  let (state, setState) = React.useState(() => initGlobalState);

  let clearAllTimers = () => {
    [
      timers.tick,
      timers.moveLeft,
      timers.moveRight,
      timers.rotate,
      timers.countdown,
    ]
    ->(Belt.List.forEach(t => Utils.clearIntervalId(t)));
  };

  let moveBlock = direction => {
    setState(state =>
      {
        ...state,
        gridState: Func.getGridStateAfterMove(direction, state.gridState),
      }
    );
  };

  let rotateBlock = () => {
    setState(state =>
      {...state, gridState: Func.getGridStateAfterRotate(state.gridState)}
    );
  };

  let tick = () => {
    setState(state => {
      let next =
        Func.tick(
          state.gridState,
          state.stats,
          ~nextBlock=state.nextBlock,
          (),
        );

      let {gridState, stats, gameOver, nextBlockToShow} = next;

      if (gameOver) {
        clearAllTimers();
        setScreen(_ => Gameover);
        {...state, gameOver: true};
      } else {
        let nextState = {
          ...state,
          nextBlock: nextBlockToShow,
          gridState,
          stats,
        };

        if (state.stats.level < next.stats.level) {
          nextState;
        } else {
          nextState;
        };
      };
    });
  };

  let updateTimer = (timerId, callback, delay) => {
    Utils.clearIntervalId(timerId);
    let newTimerId = Js.Global.setInterval(() => callback(), delay);
    timerId := Some(newTimerId);
  };

  let keyDownHandler = event => {
    let keyRepeated = Utils.Dom.isKeyRepeated(event);

    if (!keyRepeated) {
      let key = Utils.Dom.getKeyName(event);

      switch (key) {
      | "ArrowLeft" =>
        updateTimer(
          timers.moveLeft,
          () => moveBlock(Left),
          Constants.moveDelay,
        )
      | "ArrowRight" =>
        updateTimer(
          timers.moveRight,
          () => moveBlock(Right),
          Constants.moveDelay,
        )
      | "ArrowUp" =>
        updateTimer(timers.rotate, rotateBlock, Constants.rotateDelay)
      | "ArrowDown" => updateTimer(timers.tick, tick, Constants.dropDelay)
      | _ => ()
      };
    };
  };

  let keyUpHandler = event => {
    let key = Utils.Dom.getKeyName(event);

    switch (key) {
    | "ArrowLeft" => Utils.clearIntervalId(timers.moveLeft)
    | "ArrowRight" => Utils.clearIntervalId(timers.moveRight)
    | "ArrowUp" => Utils.clearIntervalId(timers.rotate)
    | "ArrowDown" =>
      let delay = Func.calcDelay(state.stats.level);
      updateTimer(timers.tick, tick, delay);
    | _ => ()
    };
  };

  let addKeyboardListeners = () => {
    Document.addKeyDownEventListener(keyDownHandler, document);
    Document.addKeyUpEventListener(keyUpHandler, document);
  };

  let removeKeyboardListeners = () => {
    Document.removeKeyDownEventListener(keyDownHandler, document);
    Document.removeKeyUpEventListener(keyUpHandler, document);
  };

  let startGame = () => setScreen(_ => getNextScreen(screen));

  React.useEffect(() => {
    if (screen == Counter) {
      if (countdownCounter > 0) {
        let timerId =
          Js.Global.setInterval(
            () =>
              setCountdownCounter(countdownCounter => countdownCounter - 1),
            Constants.countDelay,
          );
        timers.countdown := Some(timerId);
      } else {
        startGame();
      };
    } else {
      ();
    };

    Some(() => Utils.clearIntervalId(timers.countdown));
  });

  React.useEffect1(
    () => {
      if (screen == Game) {
        addKeyboardListeners();
      };

      Some(() => removeKeyboardListeners());
    },
    [|screen|],
  );

  let startCountdown = () => {
    setState(_ => initGlobalState);
    setCountdownCounter(_ => 3);
    setScreen(_ => getNextScreen(screen));
  };

  // Use Tick
  React.useEffect2(
    () => {
      switch (screen) {
      | Game =>
        let delay = Func.calcDelay(state.stats.level);
        updateTimer(timers.tick, tick, delay);
      | _ => ()
      };
      Some(() => Utils.clearIntervalId(timers.tick));
    },
    (screen, state.stats.level),
  );

  let started = screen == Game || screen == Gameover;

  let gridToRender =
    started ? Func.mapBlockToGrid(state.gridState) : initGridState.grid;

  <div className="game">
    <NextBlock nextBlock={state.nextBlock} started />
    <div className="gridContainer">
      <Grid grid=gridToRender />
      <GridOverlay
        screen
        countdownCounter
        clickStart={_event => startCountdown()}
      />
    </div>
    <Stats stats={state.stats} started />
  </div>;
};
