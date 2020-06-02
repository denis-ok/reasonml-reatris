open Types;
open Belt;

[@bs.module "./NextBlock.module.css"] external styles: Js.t({..}) = "default";

[@react.component]
let make = (~nextBlock: grid) => {
  let nextBlockRows =
    nextBlock
    ->Array.mapWithIndex((i, row) => <Grid.Row key={Int.toString(i)} row />)
    ->React.array;

  <div className=styles##leftContainer> nextBlockRows </div>;
};
