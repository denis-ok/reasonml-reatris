module Const = Constants;

open Types;
open Types.GlobalState;
open Utils.React;

[@bs.val] external document: Dom.document = "document";

[@bs.module "./Game.module.css"] external styles: Js.t({..}) = "default";

let initialGridState =
  Core.genInitGridState(
    ~gridWidth=Constants.Grid.width,
    ~gridHeight=Constants.Grid.height,
  );

let initGlobalState: GlobalState.t = {
  screen: Title,
  countdownCounter: 0,
  gridState: initialGridState,
  nextBlock: Blocks.getRandomBlock(),
  stats: {
    score: 0,
    lines: 0,
    level: 1,
  },
  timers: Timer.State.empty,
};

type action =
  | SetScreen(Screen.t)
  | StartCountdown
  | Countdown
  | ScheduleTick
  | Tick
  | MoveBlock(Direction.t)
  | RotateBlock
  | InitTimer(Timer.t, int)
  | SetTimer(Timer.t, option(Js.Global.intervalId))
  | ClearTimer(Timer.t);

let reducer =
    (state: GlobalState.t, action: action)
    : ReludeReact.Reducer.update(action, GlobalState.t) =>
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
      ({send}) => send(InitTimer(Countdown, Const.countDelay)),
    )

  | Countdown =>
    if (state.countdownCounter == 1) {
      UpdateWithSideEffect(
        {...state, screen: Game},
        ({send}) => send(ClearTimer(Countdown)),
      );
    } else {
      Update({...state, countdownCounter: state.countdownCounter - 1});
    }

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

  | InitTimer(timer, delay) =>
    SideEffect(
      ({send}) => {
        let action =
          switch (timer) {
          | Tick => Tick
          | Countdown => Countdown
          | MoveLeft => MoveBlock(Left)
          | MoveRight => MoveBlock(Right)
          | Rotate => RotateBlock
          };

        let intervalId = Js.Global.setInterval(() => send(action), delay);

        send(SetTimer(timer, Some(intervalId)));
      },
    )

  | SetTimer(timer, mbTimerId) =>
    let newTimers =
      switch (timer) {
      | Tick => {...state.timers, tick: mbTimerId}
      | Countdown => {...state.timers, countdown: mbTimerId}
      | MoveLeft => {...state.timers, moveLeft: mbTimerId}
      | MoveRight => {...state.timers, moveRight: mbTimerId}
      | Rotate => {...state.timers, rotate: mbTimerId}
      };

    Update({...state, timers: newTimers});

  | ClearTimer(timer) =>
    SideEffect(
      ({send}) => {
        let intervalId =
          switch (timer) {
          | Tick => state.timers.tick
          | Countdown => state.timers.countdown
          | MoveLeft => state.timers.moveLeft
          | MoveRight => state.timers.moveRight
          | Rotate => state.timers.rotate
          };

        Utils.clearOptionalIntervalId(intervalId);
        send(SetTimer(timer, None));
      },
    )

  | ScheduleTick =>
    SideEffect(
      ({send}) => {
        let delay = Core.calcDelay(state.stats.level);
        send(InitTimer(Tick, delay));
      },
    )
  };

[@react.component]
let make = () => {
  let ({countdownCounter, screen, gridState, nextBlock, stats}, send) =
    ReludeReact.Reducer.useReducer(reducer, initGlobalState);

  let keyDownHandler = event => {
    let keyRepeated = Utils.Dom.isKeyRepeated(event);

    if (!keyRepeated) {
      let pressedButton =
        Utils.Dom.getKeyName(event)->KeyboardButton.fromString;

      switch (pressedButton) {
      | ArrowLeft => InitTimer(MoveLeft, Const.moveDelay)->send
      | ArrowRight => InitTimer(MoveRight, Const.moveDelay)->send
      | ArrowUp => InitTimer(Rotate, Const.rotateDelay)->send
      | ArrowDown =>
        ClearTimer(Tick)->send;
        InitTimer(Tick, Const.dropDelay)->send;
      | Unsupported => ()
      };
    };
  };

  let keyUpHandler = event => {
    let pressedButton =
      Utils.Dom.getKeyName(event)->KeyboardButton.fromString;

    switch (pressedButton) {
    | ArrowLeft => ClearTimer(MoveLeft)->send
    | ArrowRight => ClearTimer(MoveRight)->send
    | ArrowUp => ClearTimer(Rotate)->send
    | ArrowDown =>
      ClearTimer(Tick)->send;
      ScheduleTick->send;
    | Unsupported => ()
    };
  };

  let (keyUpHandler, _) = React.useState(_ => keyUpHandler);
  let (keyDownHandler, _) = React.useState(_ => keyDownHandler);

  React.useEffect1(
    () => {
      switch (screen) {
      | Game =>
        Utils.Dom.addKeyboardListeners(
          ~keyDownHandler,
          ~keyUpHandler,
          document,
        );

        ScheduleTick->send;

      | Gameover =>
        Utils.Dom.removeKeyboardListeners(
          ~keyDownHandler,
          ~keyUpHandler,
          document,
        );

        ClearTimer(Tick)->send;
        ClearTimer(MoveLeft)->send;
        ClearTimer(MoveRight)->send;
        ClearTimer(Rotate)->send;
      | _ => ()
      };

      None;
    },
    [|screen|],
  );

  React.useEffect1(
    () => {
      if (stats.level > 1) {
        ClearTimer(Tick)->send;
        ScheduleTick->send;
      };

      None;
    },
    [|stats.level|],
  );

  let started = screen == Game || screen == Gameover;

  let gridToRender =
    started ? Core.mapBlockToGrid(gridState) : initialGridState.grid;

  let onClickStart = () => send(StartCountdown);

  <div className=styles##game>
    {started &&& <NextBlock nextBlock />}
    <div className=styles##gridContainer>
      <Grid grid=gridToRender />
      <GridOverlay screen countdownCounter onClickStart />
    </div>
    {started &&& <Stats stats />}
  </div>;
};
