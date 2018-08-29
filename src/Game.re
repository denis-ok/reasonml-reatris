open Types;

[@bs.val] external document : Dom.document = "document";

let emptyRow = Array.make(10, O);

let emptyGrid = Array.make(22, emptyRow);

let firstBlock = Blocks.getRandomBlock();

let firstNextBlock = Blocks.getRandomBlock();

let initBlockPosition = Functions.genInitBlockPosition(firstBlock, emptyGrid);

let initStats = {score: 0, lines: 0, level: 1};

let initGridState = {
  block: firstBlock,
  blockPosition: initBlockPosition,
  grid: emptyGrid,
};

let initTimerIds = {
  tick: ref(None),
  countdown: ref(None),
  moveLeft: ref(None),
  moveRight: ref(None),
  rotate: ref(None),
};

let initGlobalState: globalState = {
  gridState: initGridState,
  nextBlock: firstNextBlock,
  countdownCounter: 0,
  stats: initStats,
  timerIds: initTimerIds,
  started: false,
  gameOver: false
};

let clearIntervalId = (id: intervalId) =>
  switch (id^) {
  | Some(id) => Js.Global.clearInterval(id)
  | None => ()
  };

type state = globalState;

type action =
  | StartGame
  | StartCountdown
  | Countdown
  | Tick
  | MoveBlock(direction)
  | RotateBlock
  | UpdateLevel;

type self = ReasonReact.self(state, ReasonReact.noRetainedProps, action);

let keyDownToAction = (event, self: self) => {
  let key = event |> Webapi.Dom.KeyboardEvent.key;
  let repeated = event |> Webapi.Dom.KeyboardEvent.repeat;
  let started = self.state.started;
  let gameOver = self.state.gameOver;

  let dropDelay = 30;
  let moveDelay = 50;
  let rotateDelay = 90;

  let setIntervalForAction = (action, timerId, delay) => {
    clearIntervalId(timerId);
    let id = Js.Global.setInterval(() => self.ReasonReact.send(action), delay);
    timerId := Some(id);
  };

  if (! started || gameOver || repeated) {
    ();
  } else {
    switch (key) {
    | "ArrowDown" =>
      let tickId = self.state.timerIds.tick;
      setIntervalForAction(Tick, tickId, dropDelay);

    | "ArrowLeft" =>
      let moveLeftId = self.state.timerIds.moveLeft;
      setIntervalForAction(MoveBlock(Left), moveLeftId, moveDelay);

    | "ArrowRight" =>
      let moveRightId = self.state.timerIds.moveRight;
      setIntervalForAction(MoveBlock(Right), moveRightId, moveDelay);

    | "ArrowUp" =>
      let rotateId = self.state.timerIds.rotate;
      setIntervalForAction(RotateBlock, rotateId, rotateDelay);

    | _ => ()
    };
  };
};

let clickStart = (_event, self: self) =>
  self.ReasonReact.send(StartCountdown);

let keyUpToAction = (event, self: self) => {
  let key = event |> Webapi.Dom.KeyboardEvent.key;
  let started = self.state.started;

  if (started) {
    switch (key) {
    | "ArrowLeft" => clearIntervalId(self.state.timerIds.moveLeft)
    | "ArrowRight" => clearIntervalId(self.state.timerIds.moveRight)
    | "ArrowUp" => clearIntervalId(self.state.timerIds.rotate)
    | "ArrowDown" =>
      let delay = Functions.calcDelay(self.state.stats.level);
      let timerId = self.state.timerIds.tick;

      clearIntervalId(timerId);
      let intervalId = Js.Global.setInterval(() => self.send(Tick), delay);
      timerId := Some(intervalId);

    | _ => ()
    };
  } else {
    ();
  };
};

let addKeyDownEventListener = Webapi.Dom.Document.addKeyDownEventListener;
let addKeyUpEventListener = Webapi.Dom.Document.addKeyUpEventListener;

let component = ReasonReact.reducerComponent("Game");

let make = _children => {
  ...component,
  initialState: () => initGlobalState,

  didMount: self => {
    addKeyDownEventListener(self.handle(keyDownToAction), document);
    addKeyUpEventListener(self.handle(keyUpToAction), document);
  },

  reducer: (action, state) =>
    switch (action) {
    | StartGame =>
      let delay = Functions.calcDelay(state.stats.level);

      ReasonReact.UpdateWithSideEffects(
        {...state, started: true},
        (
          self => {
            let timerId = state.timerIds.tick;
            let intervalId = Js.Global.setInterval(() => self.send(Tick), delay);
            timerId := Some(intervalId);
          }
        ),
      );

    | StartCountdown =>
      ReasonReact.UpdateWithSideEffects(
        {...state, countdownCounter: 3},
        (
          self => {
            let timerId = state.timerIds.countdown;
            let countdownId = Js.Global.setInterval(() => self.send(Countdown), 1000);
            timerId := Some(countdownId);
          }
        ),
      )

    | Countdown =>
      let counter = state.countdownCounter;

      if (counter > 1) {
        ReasonReact.Update({
          ...state,
          countdownCounter: state.countdownCounter - 1,
        });
      } else {
        ReasonReact.UpdateWithSideEffects(
          {...state, countdownCounter: 0},
          (
            self => {
              clearIntervalId(state.timerIds.countdown);
              self.send(StartGame);
            }
          ),
        );
      };

    | MoveBlock(direction) =>
      ReasonReact.Update({
        ...state,
        gridState: Functions.getGridStateAfterMove(direction, state.gridState),
      })

    | RotateBlock =>
      ReasonReact.Update({...state, gridState: Functions.getGridStateAfterRotate(state.gridState)})

    | UpdateLevel =>
      let delay = Functions.calcDelay(state.stats.level);
      let tickId = state.timerIds.tick;
      clearIntervalId(tickId);

      ReasonReact.SideEffects(
        (
          self => {
            let intervalId =  Js.Global.setInterval(() => self.send(Tick), delay);
            tickId := Some(intervalId);
          }
        ),
      );

    | Tick =>
      let next = Functions.tick(state.gridState, state.nextBlock, state.stats);

      let nextBlock =
        if (next.gridState.block === state.nextBlock) {
          Blocks.getRandomBlock();
        } else {
          state.nextBlock;
        };

      if (next.gameOver) {
        Js.log("Game over!");
        clearIntervalId(state.timerIds.tick);
        ReasonReact.Update({...state, gameOver: true});
      } else {
        let nextState = {
          ...state,
          gridState: next.gridState,
          stats: next.stats,
          nextBlock
        };

        if (state.stats.level < next.stats.level) {
          ReasonReact.UpdateWithSideEffects(
            nextState,
            (self => self.send(UpdateLevel)),
          );
        } else {
          ReasonReact.Update(nextState);
        };
      };
    },

  render: self => {
    let {nextBlock, gridState, countdownCounter, stats, started} = self.state;
    let gridToRender = started ? Functions.mapBlockToGrid(gridState) : emptyGrid;

    <div className="Game">
      <Grid.NextBlockArea nextBlock started />
      <Grid.GameArea
        grid=gridToRender
        counter=countdownCounter
        started
        clickStart=(self.handle(clickStart))
      />
      <Stats.StatsContainer stats started />
    </div>;
  },
};
