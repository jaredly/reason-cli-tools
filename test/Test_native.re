
open Test_shared;

/* Pollable command */
let job = Commands.exec(~cmd="yes", ~onOut=line => ());
let t = Utils.now();
while (Utils.now() -. t < 0.01) {
  Commands.poll(job);
};
Commands.kill(job);

/* Keepalive */
let starts = ref(0);
let job = Commands.keepAlive(~cmd="yes", ~checkInterval=0.01, ~onStart=() => starts := starts^ + 1, ());
let t = Utils.now();
while (Utils.now() -. t < 0.2) {
  Commands.poll(job);
};
Commands.kill(job);
ensure(starts^ == 1, "yes starts only once");

let starts = ref(0);
let job = Commands.keepAlive(~cmd="echo 'hi'", ~checkInterval=0.05, ~onStart=() => starts := starts^ + 1, ());
let t = Utils.now();
while (Utils.now() -. t < 2.) {
  Commands.poll(job);
};
Commands.kill(job);
ensure(starts^ > 1, "echo starts a bunch");

report();