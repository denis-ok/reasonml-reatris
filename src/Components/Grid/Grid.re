open Types;

let styles = [%raw {|require('./Grid.module.css')|}];

module Tile = {
  [@react.component]
  let make = (~cell: cell) => {
    let className =
      switch (cell) {
      | O => styles##cell
      | X => styles##tile
      };

    <div className />;
  };
};

module Row = {
  [@react.component]
  let make = (~row: row) => {
    <div className=styles##gridRow>
      {row
       ->Belt.Array.mapWithIndex((i, cell) =>
           <Tile key={string_of_int(i)} cell />
         )
       ->React.array}
    </div>;
  };
};

[@react.component]
let make = (~grid: grid) => {
  <div className=styles##grid>
    {grid
     ->Belt.Array.mapWithIndex((i, row) =>
         i > 1 ? <Row key={string_of_int(i)} row /> : React.null
       )
     ->React.array}
  </div>;
};
