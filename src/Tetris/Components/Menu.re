[%bs.raw {|require('./Menu.css')|}];

let component = ReasonReact.statelessComponent("Menu");

let make = (~started, _children) => {
  ...component,
  render: _self =>
    if (started) {
      ReasonReact.null;
    } else {
      <div>
        <p className="Menu-item Start">(ReasonReact.string("Start Game")) </p>
        <p className="Menu-item Level"> (ReasonReact.string("Level:")) </p>
      </div>;
    },
};
