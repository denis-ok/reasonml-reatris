open Types;
open Belt;

[@bs.module "./Grid.module.css"] external styles: Js.t({..}) = "default";

module Tile = {
  [@react.component]
  let make = (~cell: Cell.t) => {
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
       ->Array.mapWithIndex((i, cell) => <Tile key={Int.toString(i)} cell />)
       ->React.array}
    </div>;
  };
};

[@react.component]
let make = (~grid: grid) => {
  <div className=styles##grid>
    {grid
     ->Array.mapWithIndex((i, row) =>
         i >= Constants.Grid.hiddenRowsCount - 1
           ? <Row key={Int.toString(i)} row /> : React.null
       )
     ->React.array}
  </div>;
};
