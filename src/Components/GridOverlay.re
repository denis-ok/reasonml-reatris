open Types;

[%bs.raw {|require('./gridOverlay.css')|}];

module TitleScreen = {
  let component = ReasonReact.statelessComponent("TitleScreen");

  let make = (~clickStart, _children) => {
    ...component,
    render: _self =>
      <div className="grid-overlay">
        <h2 className="title-heading"> (ReasonReact.string("REATRIS")) </h2>
        <div className="btn" onClick=clickStart>
          <p> (ReasonReact.string("Start Game")) </p>
        </div>
      </div>,
  };
};

module CounterScreen = {
  let component = ReasonReact.statelessComponent("CounterScreen");

  let make = (~countdownCounter: countdownCounter, _children) => {
    ...component,
    render: _self =>
      <div className="grid-overlay">
        <p className="number">
          (ReasonReact.string(string_of_int(countdownCounter)))
        </p>
      </div>,
  };
};

module GameoverScreen = {
  let component = ReasonReact.statelessComponent("GameoverScreen");

  let make = (~clickStart, _children) => {
    ...component,
    render: _self =>
      <div className="grid-overlay">
        <div className="btn" onClick=clickStart>
          <p> (ReasonReact.string("Play Again?")) </p>
        </div>
      </div>,
  };
};

let component = ReasonReact.statelessComponent("GridOverlay");

let make = (~screen, ~countdownCounter, ~clickStart, _children) => {
  ...component,
  render: _self =>
    switch (screen) {
    | Title => <TitleScreen clickStart />
    | Counter => <CounterScreen countdownCounter />
    | Game => ReasonReact.null
    | Gameover => <GameoverScreen clickStart />
    },
};
