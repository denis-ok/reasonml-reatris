module RR = ReasonReact;
open Types;

[%bs.raw {|require('./gridOverlay.css')|}];

module TitleScreen = {
  let component = RR.statelessComponent("TitleScreen");

  let make = (~clickStart, _children) => {
    ...component,
    render: _self =>
      <div className="grid-overlay">
        <h2 className="title-heading"> {RR.string("REATRIS")} </h2>
        <div className="btn" onClick=clickStart>
          <p> {RR.string("Start Game")} </p>
        </div>
      </div>,
  };
};

module CounterScreen = {
  let component = RR.statelessComponent("CounterScreen");

  let make = (~countdownCounter: countdownCounter, _children) => {
    ...component,
    render: _self =>
      <div className="grid-overlay">
        <p className="number">
          {RR.string(string_of_int(countdownCounter))}
        </p>
      </div>,
  };
};

module GameoverScreen = {
  let component = RR.statelessComponent("GameoverScreen");

  let make = (~clickStart, _children) => {
    ...component,
    render: _self =>
      <div className="grid-overlay">
        <div className="btn" onClick=clickStart>
          <p> {RR.string("Play Again?")} </p>
        </div>
      </div>,
  };
};

let component = RR.statelessComponent("GridOverlay");

let make = (~screen, ~countdownCounter, ~clickStart, _children) => {
  ...component,
  render: _self =>
    switch (screen) {
    | Title => <TitleScreen clickStart />
    | Counter => <CounterScreen countdownCounter />
    | Game => RR.null
    | Gameover => <GameoverScreen clickStart />
    },
};
