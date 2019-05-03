[@bs.config {jsx: 3}];

module RR = ReasonReact;
open Types;

[%bs.raw {|require('./grid.css')|}];

module Tile = {
  [@react.component]
  let make = (~cell: cell) => {
    let klass =
      switch (cell) {
      | O => "cell"
      | X => "tile"
      };

    <div className=klass />;
  };
};

module Row = {
  [@react.component]
  let make = (~row: row) => {
    <div className="gridRow">
      {row
       |> Array.mapi((i, cell) => <Tile key={string_of_int(i)} cell />)
       |> RR.array}
    </div>;
  };
};

[@react.component]
let make = (~grid: grid) => {
  <div className="grid">
    {grid
     |> Array.mapi((i, row) =>
          i > 1 ? <Row key={string_of_int(i)} row /> : RR.null
        )
     |> RR.array}
  </div>;
};

module Jsx2 = {
  let component = ReasonReact.statelessComponent("Grid");
  let make = (~grid: grid, children) =>
    ReasonReactCompat.wrapReactForReasonReact(
      make,
      makeProps(~grid, ()),
      children,
    );
};
