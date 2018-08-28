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
        (
          row
          |> Array.mapi((i, cell) => <Tile key=(string_of_int(i)) cell />)
          |> ReasonReact.array
        )
      </div>,
  };
};

module NextBlockArea = {
  let component = ReasonReact.statelessComponent("NextBlockArea");

  let make = (~nextBlock: grid, ~started, _children) => {
    ...component,
    render: _self =>
      <div className="NextBlock-container Container">
        (
          if (started) {
            <div className="NextBlock">
              (
                nextBlock
                |> Array.mapi((i, row) =>
                     <Row key=(string_of_int(i)) row />
                   )
                |> ReasonReact.array
              )
            </div>;
          } else {
            ReasonReact.null;
          }
        )
      </div>,
  };
};

module CountdownCounter = {
  let component = ReasonReact.statelessComponent("CounterSquare");

  let make = (~counter: countdownCounter, _children) => {
    ...component,
    render: _self =>
      if (counter == 0) {
        ReasonReact.null;
      } else {
        <div className="Counter">
          <div>
            <p className="Number">
              (ReasonReact.string(string_of_int(counter)))
            </p>
          </div>
        </div>;
      },
  };
};

module GameArea = {
  let component = ReasonReact.statelessComponent("Grid");

  let make = (~grid: grid, ~counter: countdownCounter, ~started, _children) => {
    ...component,
    render: _self =>
      <div className="Grid-container Container">
        <div className="Grid">
          (
            grid
            |> Array.mapi((i, row) =>
                 i > 1 ? <Row key=(string_of_int(i)) row /> : ReasonReact.null
               )
            |> ReasonReact.array
          )
          <CountdownCounter counter />
        </div>
        <Menu started />
      </div>,
  };
};
