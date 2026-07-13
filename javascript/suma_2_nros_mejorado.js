
const readline = require('readline');

// Usar async/await hace el código más legible y evita el "Callback Hell".
async function main() {
    const rl = readline.createInterface({
        input: process.stdin,
        output: process.stdout
    });

    // Creamos una función auxiliar que convierte el callback de rl.question en una Promesa.
    const question = (query) => new Promise(resolve => rl.question(query, resolve));

    try {
        const valor1Str = await question('Ingresa el primer valor: ');
        const valor2Str = await question('Ingresa el segundo valor: ');

        // Convierte las entradas a números.
        const valor1 = parseFloat(valor1Str);
        const valor2 = parseFloat(valor2Str);

        // Verificamos si la conversión fue exitosa.
        if (isNaN(valor1) || isNaN(valor2)) {
            console.error('Error: Uno o ambos valores no son números válidos.');
        } else {
            console.log('Variable 1:', valor1);
            console.log('Variable 2:', valor2);
            
            // Realizamos la operación.
            const suma = valor1 + valor2;
            console.log("La suma de las variables es: ", suma);
        }
    } catch (error) {
        console.error("Ocurrió un error:", error);
    } finally {
        rl.close(); // Aseguramos que la interfaz se cierre siempre.
    }
}

main();
