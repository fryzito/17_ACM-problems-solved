console.log('hola mundo primer programa en javascript puro');
const readline = require('readline');

const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});

rl.question('Ingresa el primer valor: ', (valor1) => {
    rl.question('Ingresa el segundo valor: ', (valor2) => {
	// Convierte las entradas a nmeros si es necesario
	valor1 = parseFloat(valor1);
	valor2 = parseFloat(valor2);

	console.log('Variable 1:', valor1);
	console.log('Variable 2:', valor2);
	//puedes realizar operaciones con las variables
	let suma = valor1 + valor2;
	console.log("La suma de las variables es: ", suma)

	rl.close();
    });
});
