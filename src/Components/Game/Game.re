module Const = Constants;

open Types;
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
  stats: GameStats.t,
  gridState: GridState.t,
  nextBlock: block,
};

let initState2 = {
  screen: Title,
  countdownCounter: 0,
  countdownIntervalId: None,
  stats: {
    score: 0,
    lines: 0,
    level: 1,
  },
  gridState: initialGridState,
  nextBlock: Blocks.getRandomBlock(),
};

type action =
  | SetScreen(Screen.t)
  | StartCountdown
  | Countdown
  | SetCountdownIntervalId(option(Js.Global.intervalId))
  | Tick
  | MoveBlock(Direction.t)
  | RotateBlock;

let reducer =
    (state: state2, action: action)
    : ReludeReact.Reducer.update(action, state2) =>
  switch (action) {
  | SetScreen(screen) => Update({...state, screen})

  | StartCountdown =>
    UpdateWithSideEffect(
      {
        ...state,
        screen: Counter,
        countdownCounter: 3,
        gridState: initialGridState,
        stats: {
          score: 0,
          lines: 0,
          level: 1,
        },
        nextBlock: Blocks.getRandomBlock(),
      },
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

  | Tick =>
    let next =
      Core.tick(state.gridState, state.stats, ~nextBlock=state.nextBlock, ());

    let {gridState, stats, gameOver, nextBlockToShow}: TickOutput.t = next;

    if (gameOver) {
      Update({...state, screen: Gameover});
    } else {
      Update({...state, nextBlock: nextBlockToShow, gridState, stats});
    };

  | MoveBlock(direction) =>
    let nextGridState =
      Core.getGridStateAfterMove(direction, state.gridState);

    Update({...state, gridState: nextGridState});

  | RotateBlock =>
    let nextGridState = Core.getGridStateAfterRotate(state.gridState);
    Update({...state, gridState: nextGridState});
  };

[@react.component]
let make = () => {
  let ({countdownCounter, screen, gridState, nextBlock, stats}, send) =
    ReludeReact.Reducer.useReducer(reducer, initState2);

  let (timers: timerIds, _setTimer) = React.useState(() => initTimerIds);

  let tick = () => send(Tick);

  // let clearAllTimers = () => {
  //   [
  //     timers.tick,
  //     timers.moveLeft,
  //     timers.moveRight,
  //     timers.rotate,
  //     timers.countdown,
  //   ]
  //   ->(List.forEach(Utils.clearIntervalId));
  // };

  let moveBlock = direction => send(MoveBlock(direction));

  let rotateBlock = () => send(RotateBlock);

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
      let delay = Core.calcDelay(stats.level);
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

  React.useEffect1(
    () => {
      switch (screen) {
      | Game =>
        let delay = Core.calcDelay(stats.level);

        updateTimer(timers.tick, tick, delay);
        None;
      | Gameover => Some(() => Utils.clearIntervalId(timers.tick))
      | _ => None
      }
    },
    [|screen|],
  );

  let started = screen == Game || screen == Gameover;

  let gridToRender =
    started ? Core.mapBlockToGrid(gridState) : initialGridState.grid;

  <div className=styles##game>
    {started &&& <NextBlock nextBlock />}
    <div className=styles##gridContainer>
      <Grid grid=gridToRender />
      <GridOverlay
        screen
        countdownCounter
        clickStart={_event => send(StartCountdown)}
      />
    </div>
    {started &&& <Stats stats />}
  </div>;
};
