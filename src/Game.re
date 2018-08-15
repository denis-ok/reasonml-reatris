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

let initGameState = {
  gridState: initGridState,
  gameOver: false,
  intervalId: None,
};

type state = gameState;

type action =
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
  | _ => ()
  };
};

let addKeyDownEventListener = Webapi.Dom.Document.addKeyDownEventListener;

let component = ReasonReact.reducerComponent("Game");

let make = _children => {
  ...component,
  initialState: () => initGameState,

  didMount: self => {
    let intervalId = Js.Global.setInterval(() => self.send(Tick), 500);
    self.send(SetIntervalId(intervalId));

    addKeyDownEventListener(self.handle(keyToAction), document);
  },

  reducer: (action, state) =>
    switch (action) {
    | SetIntervalId(id) =>
      ReasonReact.Update({...state, intervalId: Some(id)})

    | MoveBlock(direction) =>
      ReasonReact.Update({...state, gridState: Functions.getGridStateAfterMove(direction, state.gridState)});

    | RotateBlock =>
      ReasonReact.Update({...state, gridState: Functions.getGridStateAfterRotate(state.gridState)});

    | Tick =>
      let nextGridState = Functions.tick(state.gridState);
      let gameOver = Functions.isGameOver(nextGridState);

      switch (gameOver) {
      | false => ReasonReact.Update({...state, gridState: nextGridState})
      | true =>
        Js.log("Game over!");
        switch (state.intervalId) {
        | Some(id) => Js.Global.clearInterval(id)
        | None => ()
        };
        ReasonReact.NoUpdate;
      };
    },

  render: self => {
    let gridToRender = Functions.mapBlockToGridOk(self.state.gridState);
    <div className="Game">
      <Grid grid=gridToRender />
    </div>;
  },
};
