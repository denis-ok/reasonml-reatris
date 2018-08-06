open Types;
open Blocks;

let initBlockPosition = {x: 0, y: 0};

let emptyRow = Array.make(10, O);

let emptyGrid = Array.make(10, emptyRow);

let initGridState = {
  block: blockT,
  blockPosition: initBlockPosition,
  grid: emptyGrid,
};

let initGameState = {
  block: blockT,
  gridState: initGridState,
  gameOver: false,
  intervalId: None,
};

type state = gameState;

type action =
  | SetIntervalId(Js.Global.intervalId)
  | Tick;

let component = ReasonReact.reducerComponent("Game");

let make = _children => {
  ...component,
  initialState: () => initGameState,

  didMount: self => {
    let intervalId = Js.Global.setInterval(() => self.send(Tick), 100);
    self.send(SetIntervalId(intervalId));
  },

  reducer: (action, state) =>
    switch (action) {
    | SetIntervalId(id) => ReasonReact.Update({...state, intervalId: Some(id)})
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
        ReasonReact.Update({...state, gameOver: true});
      };
    },

  render: self => {
    let gridToRender = Functions.mapBlockToGridOk(self.state.gridState);
    <div className="Game"> <Grid grid=gridToRender /> </div>;
  },
};
