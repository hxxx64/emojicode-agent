const Cat  = require('./cat');
const Dog  = require('./dog');
const Fish = require('./fish');

console.log('Hello, World!');

const cat  = new Cat('Whiskers');
const dog  = new Dog('Rex');
const fish = new Fish('Nemo');

cat.meow();
cat.sleep();
dog.bark();
fish.swim();
fish.eat();
