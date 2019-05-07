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
       ->Belt.Array.mapWithIndex((i, cell) =>
           <Tile key={string_of_int(i)} cell />
         )
       ->React.array}
    </div>;
  };
};

[@react.component]
let make = (~grid: grid) => {
  <div className="grid">
    {grid
     ->Belt.Array.mapWithIndex((i, row) =>
         i > 1 ? <Row key={string_of_int(i)} row /> : React.null
       )
     ->React.array}
  </div>;
};
