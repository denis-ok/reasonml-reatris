open Types;

[@bs.val] external document : Dom.document = "document";

let emptyRow = Array.make(10, O);

let emptyGrid = Array.make(22, emptyRow);

let firstBlock = Blocks.getRandomBlock();

let initBlockPosition = Functions.genInitBlockPosition(firstBlock, emptyGrid);

let initStats = {
  score: 0,
  lines: 0,
  level: 1
};

let initGridState = {
  block: firstBlock,
  blockPosition: initBlockPosition,
  grid: emptyGrid,
};

let initGlobalState : globalState = {
  gridState: initGridState,
  nextBlock: Blocks.getRandomBlock(),
  stats: initStats,
  countdownCounter: 3,
  intervalId: ref(None),
  countdownId: ref(None),
};

let clearIntervalId = (id : intervalId) => {
  switch (id^) {
    | Some(id) => Js.Global.clearInterval(id)
    | None => ()
    };
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

let keyToAction = (event, self) => {
  let key = event |> Webapi.Dom.KeyboardEvent.key;

  switch (key) {
  | "ArrowLeft" => self.ReasonReact.send(MoveBlock(Left))
  | "ArrowRight" => self.ReasonReact.send(MoveBlock(Right))
  | "ArrowDown" => self.ReasonReact.send(Tick)
  | "ArrowUp" => self.ReasonReact.send(RotateBlock)
  | "Enter" => self.ReasonReact.send(StartCountdown)
  | _ => ()
  };
};

let addKeyDownEventListener = Webapi.Dom.Document.addKeyDownEventListener;

let component = ReasonReact.reducerComponent("Game");

let make = _children => {
  ...component,
  initialState: () => initGlobalState,

  didMount: self => {
    addKeyDownEventListener(self.handle(keyToAction), document);
  },

  reducer: (action, state) =>
    switch (action) {
    | StartGame => {
      let delay = Functions.calcDelay(state.stats.level);
      ReasonReact.SideEffects(self => {
        let intervalId = Js.Global.setInterval(() => self.send(Tick), delay);
        state.intervalId := Some(intervalId);
      });
    }

    | StartCountdown => {
      ReasonReact.SideEffects(self => {
        let countdownId = Js.Global.setInterval(() => self.send(Countdown), 1000);
        state.countdownId := Some(countdownId);
      });
    }

    | Countdown => {
      let counter = state.countdownCounter;
      /* Js.log(counter); */

      if (counter > 0) {
        ReasonReact.Update({...state, countdownCounter: state.countdownCounter - 1})
      } else {
        ReasonReact.SideEffects(self => {
          clearIntervalId(state.countdownId);
          self.send(StartGame);
        });
      }
    }

    | MoveBlock(direction) =>
      ReasonReact.Update({...state, gridState: Functions.getGridStateAfterMove(direction, state.gridState)});

    | RotateBlock =>
      ReasonReact.Update({...state, gridState: Functions.getGridStateAfterRotate(state.gridState)});

    | UpdateLevel => {
      let delay = Functions.calcDelay(state.stats.level);

      clearIntervalId(state.intervalId);

      ReasonReact.SideEffects(self => {
        let intervalId = Js.Global.setInterval(() => self.send(Tick), delay);
        state.intervalId := Some(intervalId);
      });
    };


    | Tick =>
      let nextGameState = Functions.tick(state.gridState, state.nextBlock, state.stats);

      let { gridState, stats, gameOver } = nextGameState;

      let nextBlock = {
        if (gridState.block === state.nextBlock) {
          Blocks.getRandomBlock()
        } else {
          state.nextBlock
        }
      };

      if (gameOver) {
        Js.log("Game over!");
        clearIntervalId(state.intervalId);
        ReasonReact.NoUpdate;
      } else {
        let nextState = {...state, gridState, stats, nextBlock: nextBlock};

        if (state.stats.level < stats.level) {
          ReasonReact.UpdateWithSideEffects(nextState, self => self.send(UpdateLevel));
        } else {
          ReasonReact.Update(nextState);
        }
      }
    },

  render: self => {
    let gridToRender = Functions.mapBlockToGrid(self.state.gridState);

    <div className="Game">
      <Grid.NextBlockArea grid=self.state.nextBlock />
      <Grid.GameArea grid=gridToRender counter=self.state.countdownCounter />
      <Stats.StatsContainer stats=self.state.stats />
    </div>
  },
};
