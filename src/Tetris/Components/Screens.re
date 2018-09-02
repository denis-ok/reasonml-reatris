open Types;

[%bs.raw {|require('./screens.css')|}];

module TitleScreen = {
  let component = ReasonReact.statelessComponent("TitleScreen");

  let make = (~clickStart, _children) => {
    ...component,
    render: _self =>
        <div>
          <p onClick=clickStart className="Menu-item Start">
            (ReasonReact.string("Start Game"))
          </p>
        </div>
  };
};

module CounterScreen = {
  let component = ReasonReact.statelessComponent("CounterScreen");

  let make = (~countdownCounter: countdownCounter, _children) => {
    ...component,
    render: _self =>
        <div className="Counter">
          <div>
            <p className="Number">
              (ReasonReact.string(string_of_int(countdownCounter)))
            </p>
          </div>
        </div>
  };
};

module GameoverScreen = {
  let component = ReasonReact.statelessComponent("GameoverScreen");

  let make = (~clickStart, _children) => {
    ...component,
    render: _self =>
      <div>
        /* <p className="Heading-gameover">
          (ReasonReact.string("Game Over"))
        </p> */
        <p onClick=clickStart className="Menu-item Start">
          (ReasonReact.string("Play again?"))
        </p>
      </div>,
  };
};


  let component = ReasonReact.statelessComponent("Screens");

  let make = (~screen, ~countdownCounter, ~clickStart, _children) => {
    ...component,
    render: _self => switch screen {
      | Title => <TitleScreen clickStart/>
      | Counter => <CounterScreen countdownCounter/>
      | Game => ReasonReact.null
      | Gameover => <GameoverScreen clickStart/>
    }
  };
