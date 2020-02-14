open Types;
open Belt;

let styles = [%raw {|require('./NextBlock.module.css')|}];

[@react.component]
let make = (~nextBlock: grid, ~started) => {
  <div className=styles##leftContainer>
    {if (started) {
       nextBlock
       ->Array.mapWithIndex((i, row) =>
           <Grid.Row key={Int.toString(i)} row />
         )
       ->React.array;
     } else {
       React.null;
     }}
  </div>;
};
