

type proc;

type job = unit => unit;

type buffer;
[@bs.module "child_process"] external spawnSync:
  (~cmd: string, ~args: array(string), ~options: Js.t({. shell: Js.boolean})) => Js.t({. stdout: buffer, stderr: buffer, status: int}) = "";

[@bs.module "child_process"] external spawn:
  (~cmd: string, ~args: array(string), ~options: Js.t({. shell: Js.boolean})) => proc = "";

[@bs.send] external on: (proc, string, unit => unit) => unit = "";
[@bs.send] external kill: proc => unit = "";
[@bs.send] external onData: (proc, string, buffer => unit) => unit = "on";

[@bs.send.pipe: buffer] external toString: (string) => string = "";

let trimLastNewline = text => if (Js.String.endsWith("\n", text)) {
  Js.String.slice(~from=0, ~to_=Js.String.length(text) - 1, text)
} else {
  text
};

let execSync = (~cmd, ~onOut=a => (), ()) => {
  let res = spawnSync(~cmd, ~args=[||], ~options={"shell": Js.true_});
  (res##stdout |> toString("utf8") |> trimLastNewline |> Js.String.split("\n") |> Array.to_list, res##status == 0)
};

let exec = (~cmd, ~onOut) => {
  let proc = spawn(~cmd, ~args=[||], ~options={"shell": Js.true_});
  () => ()
};

let run = (job) => ();
let runAll = jobs => ();
let poll = (job) => ();

let keepAlive = (~cmd, ~onOut=l => (), ~onErr=a => (), ~onStart=() => (), ~checkInterval=1., ()) => {
  let save = ref(None);
  let rec start = () => {
    Js.log("starting");
    onStart();
    let proc = spawn(~cmd, ~args=[||], ~options={"shell": Js.true_});
    on(proc, "close", () => {
      Js.log("close");
      Js.Global.setTimeout(() => {
        save := Some(start());
        Js.log("AAA");
      }, int_of_float(checkInterval *. 1000.)) |> ignore;
    });
    proc
  };
  let kill = () => {
    switch save^ {
    | Some(p) => kill(p)
    | None => ()
    }
  };
  save := Some(start());
  kill
};

let kill = (job) => job();