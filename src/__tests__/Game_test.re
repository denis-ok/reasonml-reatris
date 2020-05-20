open Jest;
open Expect;
open ReactTestingLibrary;

describe("Game component", () => {
  let rendered = render(<Game />);

  let container = rendered |> container;

  test("render", () => {
    expect(container) |> toMatchSnapshot
  });
});
