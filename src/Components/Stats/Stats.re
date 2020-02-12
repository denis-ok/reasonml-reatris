open Types;

let styles = [%raw {|require('./Stats.module.css')|}];

module StatsRow = {
  [@react.component]
  let make = (~name: string, ~value: int) => {
    <div className=styles##statsRow>
      <p> {React.string(name)} </p>
      <p> {React.string(string_of_int(value))} </p>
    </div>;
  };
};

[@react.component]
let make = (~stats: stats, ~started) => {
  <div className=styles##rightContainer>
    {if (started) {
       <>
         <StatsRow name="Score" value={stats.score} />
         <StatsRow name="Lines" value={stats.lines} />
         <StatsRow name="Level" value={stats.level} />
       </>;
     } else {
       React.null;
     }}
  </div>;
};