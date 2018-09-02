open Types;

  let component = ReasonReact.statelessComponent("NextBlock");

  let make = (~nextBlock: grid, ~started, _children) => {
    ...component,
    render: _self =>
      <div className="side-container">
        (
          if (started) {
            <div className="next-block">
              (
                nextBlock
                |> Array.mapi((i, row) =>
                     <Grid.Row key=(string_of_int(i)) row />
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
