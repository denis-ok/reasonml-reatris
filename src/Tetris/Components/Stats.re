open Types;

[%bs.raw {|require('./Stats.css')|}];

module StatsRow = {
  let component = ReasonReact.statelessComponent("StatsRow");

  let make = (~name: string, ~value: int, _children) => {
    ...component,
    render: _self =>
      <div className="StatsRow">
        <p className="StatName"> (ReasonReact.string(name)) </p>
        <p className="StatValue">
          (ReasonReact.string(string_of_int(value)))
        </p>
      </div>,
  };
};

  let component = ReasonReact.statelessComponent("Stats");

  let make = (~stats: stats, ~started, _children) => {
    ...component,
    render: _self =>
      <div className="side-container">
        (
          if (started) {
            <div className="Stats">
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
