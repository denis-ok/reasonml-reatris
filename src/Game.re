open Types;

[@bs.val] external document : Dom.document = "document";

let initBlockPosition = {x: 0, y: 0};

let emptyRow = Array.make(10, O);

let emptyGrid = Array.make(20, emptyRow);

let initGridState = {
  block: Blocks.getRandomBlock(),
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
  | RotateBlock
  | NoAction;

let keyToAction = (event) => {
  let key = event |> Webapi.Dom.KeyboardEvent.key;
  let direction = switch key {
  | "ArrowLeft" => MoveBlock(Left)
  | "ArrowRight" => MoveBlock(Right)
  | "ArrowDown" => MoveBlock(Down)
  | "ArrowUp" => RotateBlock
  | _ => NoAction;
  };

  direction;
};

let keyHandler = (evt, self) => self.ReasonReact.send(keyToAction(evt));

let addKeyDownEventListener = Webapi.Dom.Document.addKeyDownEventListener;

let component = ReasonReact.reducerComponent("Game");

let make = _children => {
  ...component,
  initialState: () => initGameState,

  didMount: self => {
    let intervalId = Js.Global.setInterval(() => self.send(Tick), 200);
    self.send(SetIntervalId(intervalId));

    addKeyDownEventListener(self.handle(keyHandler), document);
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
