open Types;

let styles = [%raw {|require('./NextBlock.module.css')|}];

[@react.component]
let make = (~nextBlock: grid, ~started) => {
  <div className=styles##sideContainer>
    {if (started) {
       <div>
         {nextBlock
          ->Belt.Array.mapWithIndex((i, row) =>
              <Grid.Row key={string_of_int(i)} row />
            )
          ->React.array}
       </div>;
     } else {
       React.null;
     }}
  </div>;
};
