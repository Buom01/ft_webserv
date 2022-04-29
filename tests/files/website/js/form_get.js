const mod = require('../dist/js/modules/module');
const __GET = mod.env.__GET;

mod.start();
mod.echo('<h1>GET node</h1>');
mod.echo(`<h3>Hello ${__GET.first_name} ${__GET.last_name} !</h3>`);
mod.echo('<p><a href=\'index.js\'>Go to index</a></p>');
mod.end();
