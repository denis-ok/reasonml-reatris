open Jest;
open Expect;

test("Delay for level 1", () => {
  expect(Core.calcDelay(1)) |> toEqual(500)
});

test("Delay for level 2", () => {
  expect(Core.calcDelay(2)) |> toEqual(250)
});

test("Delay for level 3", () => {
  expect(Core.calcDelay(3)) |> toEqual(166)
});
