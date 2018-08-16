open Types;

[%bs.raw {|require('./Grid.css')|}];

module Tile = {
  let component = ReasonReact.statelessComponent("Tile");

  let make = (~cell: cell, _children) => {
    ...component,
    render: _self => {
      let klass =
        switch (cell) {
        | O => "Cell"
        | X => "Tile"
        };
      <div className=klass />;
    },
  };
};

module Row = {
  let component = ReasonReact.statelessComponent("Row");

  let make = (~row: row, _children) => {
    ...component,
    render: _self =>
      <div className="Grid-row">
        (row |> Array.mapi((i, cell) => <Tile key=string_of_int(i) cell />) |> ReasonReact.array)
      </div>,
  };
};

module NextBlockArea = {
  let component = ReasonReact.statelessComponent("NextBlockArea");

  let make = (~grid: grid, _children) => {
    ...component,
    render: _self =>
      <div className="NextBlock-container Container">
        <div className="NextBlock">
          (grid |> Array.mapi((i, row) => <Row key=string_of_int(i) row />) |> ReasonReact.array)
        </div>
      </div>,
  };
};


module GameArea = {
  let component = ReasonReact.statelessComponent("Grid");

  let make = (~grid: grid, _children) => {
    ...component,
    render: _self =>
      <div className="Grid-container Container">
        <div className="Grid">
          (grid |> Array.mapi((i, row) => i > 1 ? <Row key=string_of_int(i) row /> : ReasonReact.null) |> ReasonReact.array)
        </div>
      </div>,
  };
};
