module RR = ReasonReact;
open Types;


[%bs.raw {|require('./grid.css')|}];

module Tile = {
  let component = RR.statelessComponent("Tile");

  let make = (~cell: cell, _children) => {
    ...component,
    render: _self => {
      let klass =
        switch (cell) {
        | O => "cell"
        | X => "tile"
        };
      <div className=klass />;
    },
  };
};

module Row = {
  let component = RR.statelessComponent("Row");

  let make = (~row: row, _children) => {
    ...component,
    render: _self =>
      <div className="gridRow">
        {row
         |> Array.mapi((i, cell) => <Tile key={string_of_int(i)} cell />)
         |> RR.array}
      </div>,
  };
};

let component = RR.statelessComponent("Grid");

let make = (~grid: grid, _children) => {
  ...component,
  render: _self =>
    <div className="grid">
      {grid
       |> Array.mapi((i, row) =>
            i > 1 ? <Row key={string_of_int(i)} row /> : RR.null
          )
       |> RR.array}
    </div>,
};
