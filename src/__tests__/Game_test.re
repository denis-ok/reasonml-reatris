open Jest;
open Expect;
open ReactTestingLibrary;

describe("Game component", () => {
  test("Render game", () => {
    let rendered = render(<Game />);

    let container = rendered |> container;
    expect(container) |> toMatchSnapshot;
  });

  test("Click Start", () => {
    let rendered = render(<Game />);

    let button =
      rendered |> getByText(~matcher=`Str("Start Game"), ~options=?None);

    button |> FireEvent.click(~eventInit=?None);

    let counter = rendered |> getByText(~matcher=`Str("3"), ~options=?None);

    expect(counter) |> toMatchSnapshot;
  });
});
