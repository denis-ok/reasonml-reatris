open Types;

[@react.component]
let make = (~nextBlock: grid, ~started) => {
  <div className="sideContainer">
    {if (started) {
       <div>
         {nextBlock
          |> Array.mapi((i, row) => <Grid.Row key={string_of_int(i)} row />)
          |> React.array}
       </div>;
     } else {
       React.null;
     }}
  </div>;
};
