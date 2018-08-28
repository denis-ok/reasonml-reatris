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

let initGlobalState: globalState = {
  gridState: initGridState,
  nextBlock: firstNextBlock,
  stats: initStats,
  countdownCounter: 0,
  intervalId: ref(None),
  countdownId: ref(None),
  started: false,
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

  if (repeated) {
    ();
  } else {
    switch (key) {
    | "ArrowDown" =>
      clearIntervalId(self.state.intervalId);
      let intervalId = Js.Global.setInterval(() => self.send(Tick), 30);
      self.state.intervalId := Some(intervalId);
    | "ArrowLeft" => self.ReasonReact.send(MoveBlock(Left))
    | "ArrowRight" => self.ReasonReact.send(MoveBlock(Right))
    | "ArrowUp" => self.ReasonReact.send(RotateBlock)
    | "Enter" => self.ReasonReact.send(StartCountdown)
    | _ => ()
    };
  };
};

let clickStart = (_event, self: self) => {
  self.ReasonReact.send(StartCountdown);
};

let keyUpToAction = (event, self: self) => {
  let key = event |> Webapi.Dom.KeyboardEvent.key;

  switch (key) {
  | "ArrowDown" =>
    let delay = Functions.calcDelay(self.state.stats.level);
    clearIntervalId(self.state.intervalId);
    let intervalId = Js.Global.setInterval(() => self.send(Tick), delay);
    self.state.intervalId := Some(intervalId);
  | _ => ()
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
            let intervalId =
              Js.Global.setInterval(() => self.send(Tick), delay);
            state.intervalId := Some(intervalId);
          }
        ),
      );

    | StartCountdown =>
      ReasonReact.UpdateWithSideEffects(
        {...state, countdownCounter : 3},
        (
          self => {
            let countdownId =
              Js.Global.setInterval(() => self.send(Countdown), 1000);
            state.countdownId := Some(countdownId);
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
        ReasonReact.UpdateWithSideEffects({...state, countdownCounter: 0 },
          (
            self => {
              clearIntervalId(state.countdownId);
              self.send(StartGame);
            }
          ),
        );
      }

    | MoveBlock(direction) =>
      ReasonReact.Update({
        ...state,
        gridState:
          Functions.getGridStateAfterMove(direction, state.gridState),
      })

    | RotateBlock =>
      ReasonReact.Update({
        ...state,
        gridState: Functions.getGridStateAfterRotate(state.gridState),
      })

    | UpdateLevel =>
      let delay = Functions.calcDelay(state.stats.level);

      clearIntervalId(state.intervalId);

      ReasonReact.SideEffects(
        (
          self => {
            let intervalId =
              Js.Global.setInterval(() => self.send(Tick), delay);
            state.intervalId := Some(intervalId);
          }
        ),
      );

    | Tick =>
      let nextGameState =
        Functions.tick(state.gridState, state.nextBlock, state.stats);

      let {gridState, stats, gameOver} = nextGameState;

      let nextBlock =
        if (gridState.block === state.nextBlock) {
          Blocks.getRandomBlock();
        } else {
          state.nextBlock;
        };

      if (gameOver) {
        Js.log("Game over!");
        clearIntervalId(state.intervalId);
        ReasonReact.NoUpdate;
      } else {
        let nextState = {...state, gridState, stats, nextBlock};

        if (state.stats.level < stats.level) {
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
    let gridToRender =
      started ? Functions.mapBlockToGrid(gridState) : emptyGrid;

    <div className="Game">
      <Grid.NextBlockArea nextBlock started />
      <Grid.GameArea grid=gridToRender counter=countdownCounter started clickStart=self.handle(clickStart)/>
      <Stats.StatsContainer stats started />
    </div>;
  },
};
