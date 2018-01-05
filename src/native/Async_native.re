
type job = (unit => unit, unit => unit);

let run = ((poll, close)) => while (true) poll();

let runAll = jobs => while (true) List.iter(f => ());

let kill = ((poll, close)) => close();