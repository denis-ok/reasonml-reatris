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
};

type state = gameState;

type action =
  | Tick;

let component = ReasonReact.reducerComponent("Game");

let make = _children => {
  ...component,
  initialState: () => initGameState,
  didMount: (self) => {
    let _tick = Js.Global.setInterval(() => self.send(Tick), 500);
  },
  reducer: (action, state) =>
    switch (action) {
    | Tick =>
      let nextGridState = Functions.tick(state.gridState);
      ReasonReact.Update({...state, gridState: nextGridState});
    },
  render: self => {
    let gridToRender = Functions.mapBlockToGridOk(self.state.gridState);
    <div className="Game"> <Grid grid=gridToRender /> </div>
  }
};
