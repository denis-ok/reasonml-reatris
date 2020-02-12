open Types;

let styles = [%raw {|require('./NextBlock.module.css')|}];

[@react.component]
let make = (~nextBlock: grid, ~started) => {
  <div className=styles##leftContainer>
    {if (started) {
       nextBlock
       ->Belt.Array.mapWithIndex((i, row) =>
           <Grid.Row key={string_of_int(i)} row />
         )
       ->React.array;
     } else {
       React.null;
     }}
  </div>;
};
