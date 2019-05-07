module RR = ReasonReact;
open Types;

[@react.component]
let make = (~nextBlock: grid, ~started) => {
  <div className="sideContainer">
    {if (started) {
       <div>
         {nextBlock
          |> Array.mapi((i, row) => <Grid.Row key={string_of_int(i)} row />)
          |> RR.array}
       </div>;
     } else {
       RR.null;
     }}
  </div>;
};

module Jsx2 = {
  let component = ReasonReact.statelessComponent("NextBlock");
  let make = (~nextBlock, ~started, children) =>
    ReasonReactCompat.wrapReactForReasonReact(
      make,
      makeProps(~nextBlock, ~started, ()),
      children,
    );
};
