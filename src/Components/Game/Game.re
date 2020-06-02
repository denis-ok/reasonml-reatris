module Const = Constants;

open Types;
open Belt;

[@bs.val] external document: Dom.document = "document";

[@bs.module "./Game.module.css"] external styles: Js.t({..}) = "default";

let initialGridState =
  Core.genInitGridState(
    ~gridWidth=Constants.Grid.width,
    ~gridHeight=Constants.Grid.height,
  );

let initialGlobalState: GlobalState.t = {
  gridState: initialGridState,
  nextBlock: Blocks.getRandomBlock(),
  stats: {
    score: 0,
    lines: 0,
    level: 1,
  },
  gameOver: false,
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
  let (screen, setScreen) = React.useState(() => Screen.Title);
  let (countdownCounter, setCountdownCounter) = React.useState(() => 0);
  let (state, setState) = React.useState(() => initialGlobalState);

  let clearAllTimers = () => {
    [
      timers.tick,
      timers.moveLeft,
      timers.moveRight,
      timers.rotate,
      timers.countdown,
    ]
    ->(List.forEach(Utils.clearIntervalId));
  };

  let moveBlock = direction => {
    setState(state =>
      {
        ...state,
        gridState: Core.getGridStateAfterMove(direction, state.gridState),
      }
    );
  };

  let rotateBlock = () => {
    setState(state =>
      {...state, gridState: Core.getGridStateAfterRotate(state.gridState)}
    );
  };

  let tick = () => {
    setState(state => {
      let next =
        Core.tick(
          state.gridState,
          state.stats,
          ~nextBlock=state.nextBlock,
          (),
        );

      let {gridState, stats, gameOver, nextBlockToShow}: TickOutput.t = next;

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
      let pressedButton =
        Utils.Dom.getKeyName(event)->KeyboardButton.fromString;

      switch (pressedButton) {
      | ArrowLeft =>
        updateTimer(timers.moveLeft, () => moveBlock(Left), Const.moveDelay)
      | ArrowRight =>
        updateTimer(
          timers.moveRight,
          () => moveBlock(Right),
          Const.moveDelay,
        )
      | ArrowUp => updateTimer(timers.rotate, rotateBlock, Const.rotateDelay)
      | ArrowDown => updateTimer(timers.tick, tick, Const.dropDelay)
      | Unsupported => ()
      };
    };
  };

  let keyUpHandler = event => {
    let pressedButton =
      Utils.Dom.getKeyName(event)->KeyboardButton.fromString;

    switch (pressedButton) {
    | ArrowLeft => Utils.clearIntervalId(timers.moveLeft)
    | ArrowRight => Utils.clearIntervalId(timers.moveRight)
    | ArrowUp => Utils.clearIntervalId(timers.rotate)
    | ArrowDown =>
      let delay = Core.calcDelay(state.stats.level);
      updateTimer(timers.tick, tick, delay);
    | Unsupported => ()
    };
  };

  let startGame = () => setScreen(_ => Core.nextScreen(screen));

  React.useEffect(() => {
    if (screen == Counter) {
      if (countdownCounter > 0) {
        let timerId =
          Js.Global.setInterval(
            () =>
              setCountdownCounter(countdownCounter => countdownCounter - 1),
            Const.countDelay,
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

  //Use keyboard
  React.useEffect1(
    () =>
      if (screen == Game) {
        Utils.Dom.addKeyboardListeners(
          ~keyDownHandler,
          ~keyUpHandler,
          document,
        );

        Some(
          () =>
            Utils.Dom.removeKeyboardListeners(
              ~keyDownHandler,
              ~keyUpHandler,
              document,
            ),
        );
      } else {
        None;
      },
    [|screen|],
  );

  let startCountdown = () => {
    setState(_ => initialGlobalState);
    setCountdownCounter(_ => 3);
    setScreen(_ => Core.nextScreen(screen));
  };

  // Use Tick
  React.useEffect2(
    () => {
      switch (screen) {
      | Game =>
        let delay = Core.calcDelay(state.stats.level);
        updateTimer(timers.tick, tick, delay);
      | _ => ()
      };
      Some(() => Utils.clearIntervalId(timers.tick));
    },
    (screen, state.stats.level),
  );

  let started = screen == Game || screen == Gameover;

  let gridToRender =
    started ? Core.mapBlockToGrid(state.gridState) : initialGridState.grid;

  <div className=styles##game>
    <NextBlock nextBlock={state.nextBlock} started />
    <div className=styles##gridContainer>
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
