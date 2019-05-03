[@bs.config {jsx: 3}];

module RR = ReasonReact;
open Types;

let component = RR.statelessComponent("NextBlock");

let make = (~nextBlock: grid, ~started, _children) => {
  ...component,
  render: _self =>
    <div className="sideContainer">
      {if (started) {
         <div>
           {nextBlock
            |> Array.mapi((i, row) =>
                 <Grid.Row key={string_of_int(i)} row />
               )
            |> RR.array}
         </div>;
       } else {
         RR.null;
       }}
    </div>,
};
