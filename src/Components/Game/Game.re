module Const = Constants;

open Types;
open Belt;
open Utils.React;

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

type state2 = {
  screen: Screen.t,
  countdownCounter: int,
  countdownIntervalId: option(Js.Global.intervalId),
};

let initState2 = {
  screen: Title,
  countdownCounter: 0,
  countdownIntervalId: None,
};

type action =
  | SetScreen(Screen.t)
  | StartCountdown
  | Countdown
  | SetCountdownIntervalId(option(Js.Global.intervalId));

let reducer =
    (state: state2, action: action)
    : ReludeReact.Reducer.update(action, state2) =>
  switch (action) {
  | SetScreen(screen) => Update({...state, screen})

  | StartCountdown =>
    UpdateWithSideEffect(
      {...state, screen: Counter, countdownCounter: 3},
      ({send}) => {
        let timerId =
          Js.Global.setInterval(() => send(Countdown), Const.countDelay);

        send(SetCountdownIntervalId(Some(timerId)));
      },
    )

  | Countdown =>
    if (state.countdownCounter == 1) {
      UpdateWithSideEffect(
        {...state, screen: Game},
        ({send}) => {
          Utils.clearOptionalIntervalId(state.countdownIntervalId);
          send(SetCountdownIntervalId(None));
        },
      );
    } else {
      Update({...state, countdownCounter: state.countdownCounter - 1});
    }

  | SetCountdownIntervalId(countdownIntervalId) =>
    Update({...state, countdownIntervalId})
  };

[@react.component]
let make = () => {
  let ({countdownCounter, screen}, send) =
    ReludeReact.Reducer.useReducer(reducer, initState2);

  let setScreen = screen => send(SetScreen(screen));

  let (timers: timerIds, _setTimer) = React.useState(() => initTimerIds);
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
        setScreen(Gameover);
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
    send(StartCountdown);
  };

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
    {started &&& <NextBlock nextBlock={state.nextBlock} />}
    <div className=styles##gridContainer>
      <Grid grid=gridToRender />
      <GridOverlay
        screen
        countdownCounter
        clickStart={_event => startCountdown()}
      />
    </div>
    {started &&& <Stats stats={state.stats} />}
  </div>;
};
