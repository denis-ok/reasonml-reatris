open Types.GameStats;
open Belt;

[@bs.module "./Stats.module.css"] external styles: Js.t({..}) = "default";

module StatsRow = {
  [@react.component]
  let make = (~name: string, ~value: int) => {
    <div className=styles##statsRow>
      <p> {React.string(name)} </p>
      <p> {React.string(Int.toString(value))} </p>
    </div>;
  };
};

[@react.component]
let make = (~stats: t, ~started) =>
  if (started) {
    <div className=styles##rightContainer>
      <StatsRow name="Score" value={stats.score} />
      <StatsRow name="Lines" value={stats.lines} />
      <StatsRow name="Level" value={stats.level} />
    </div>;
  } else {
    React.null;
  };
