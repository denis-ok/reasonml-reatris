open Types;

[%bs.raw {|require('./stats.css')|}];

module StatsRow = {
  let component = ReasonReact.statelessComponent("StatsRow");

  let make = (~name: string, ~value: int, _children) => {
    ...component,
    render: _self =>
      <div className="statsRow">
        <p className="statName"> (ReasonReact.string(name)) </p>
        <p className="statValue">
          (ReasonReact.string(string_of_int(value)))
        </p>
      </div>,
  };
};

  let component = ReasonReact.statelessComponent("Stats");

  let make = (~stats: stats, ~started, _children) => {
    ...component,
    render: _self =>
      <div className="sideContainer">
        (
          if (started) {
            <div className="stats">
              <StatsRow name="Score" value=stats.score />
              <StatsRow name="Lines" value=stats.lines />
              <StatsRow name="Level" value=stats.level />
            </div>;
          } else {
            ReasonReact.null;
          }
        )
      </div>,
  };
