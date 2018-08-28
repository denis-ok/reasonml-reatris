[%bs.raw {|require('./Menu.css')|}];

let component = ReasonReact.statelessComponent("Menu");

let make = (~counter, ~started, ~clickStart, _children) => {
  ...component,
  render: _self =>
    if (started || counter > 0) {
      ReasonReact.null;
    } else {
      <div>
        <p onClick=clickStart className="Menu-item Start">(ReasonReact.string("Start Game")) </p>
        /* <p className="Menu-item Level"> (ReasonReact.string("Level:")) </p> */
      </div>;
    },
};
