open Types;

[@bs.val] external document : Dom.document = "document";

let emptyRow = Array.make(10, O);

let emptyGrid = Array.make(22, emptyRow);

let firstBlock = Blocks.getRandomBlock();

let initBlockPosition = Functions.genInitBlockPosition(firstBlock, emptyGrid);

let initGridState = {
  block: firstBlock,
  blockPosition: initBlockPosition,
  grid: emptyGrid,
};

let initGameState : gameState = {
  gridState: initGridState,
  nextBlock: Blocks.getRandomBlock(),
  gameOver: false,
  intervalId: None,
  countdownCounter: 3,
  countdownId: ref(None)
};

type state = gameState;

type action =
  | StartGame
  | StartCountdown
  | Countdown
  | SetIntervalId(Js.Global.intervalId)
  | Tick
  | MoveBlock(direction)
  | RotateBlock;

let keyToAction = (event, self) => {
  let key = event |> Webapi.Dom.KeyboardEvent.key;

  switch (key) {
  | "ArrowLeft" => self.ReasonReact.send(MoveBlock(Left))
  | "ArrowRight" => self.ReasonReact.send(MoveBlock(Right))
  | "ArrowDown" => self.ReasonReact.send(MoveBlock(Down))
  | "ArrowUp" => self.ReasonReact.send(RotateBlock)
  | "Enter" => self.ReasonReact.send(StartCountdown)
  | _ => ()
  };
};

let addKeyDownEventListener = Webapi.Dom.Document.addKeyDownEventListener;

let component = ReasonReact.reducerComponent("Game");

let make = _children => {
  ...component,
  initialState: () => initGameState,

  didMount: self => {
    addKeyDownEventListener(self.handle(keyToAction), document);
  },

  reducer: (action, state) =>
    switch (action) {
    | StartGame => {
      ReasonReact.SideEffects(self => {
        let intervalId = Js.Global.setInterval(() => self.send(Tick), 500);
        self.send(SetIntervalId(intervalId));
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
      Js.log(counter);

      if (counter > 0) {
        ReasonReact.Update({...state, countdownCounter: state.countdownCounter - 1})
      } else {
        ReasonReact.SideEffects(self => {
          switch (self.state.countdownId^) {
            | Some(id) => Js.Global.clearInterval(id)
            | None => ()
            }
          self.send(StartGame);
        });
      }
    }

    | SetIntervalId(id) =>
      ReasonReact.Update({...state, intervalId: Some(id)})

    | MoveBlock(direction) =>
      ReasonReact.Update({...state, gridState: Functions.getGridStateAfterMove(direction, state.gridState)});

    | RotateBlock =>
      ReasonReact.Update({...state, gridState: Functions.getGridStateAfterRotate(state.gridState)});

    | Tick =>
      let nextGridState = Functions.tick(state.gridState, state.nextBlock);

      let nextBlock = {
        if (nextGridState.block === state.nextBlock) {
          Blocks.getRandomBlock()
        } else {
          state.nextBlock
        }
      };

      let gameOver = Functions.isGameOver(nextGridState);

      switch (gameOver) {
      | false => ReasonReact.Update({...state, gridState: nextGridState, nextBlock: nextBlock})
      | true =>
        Js.log("Game over!");
        switch (state.intervalId) {
        | Some(id) => Js.Global.clearInterval(id)
        | None => ()
        };
        ReasonReact.Update({...state, gameOver: true});
      };
    },

  render: self => {
    let gridToRender = Functions.mapBlockToGridOk(self.state.gridState);

    <div className="Game">
      <Grid.NextBlockArea grid=self.state.nextBlock />
      <Grid.GameArea grid=gridToRender />
      <Grid.NextBlockArea grid=self.state.nextBlock />
    </div>
  },
};
