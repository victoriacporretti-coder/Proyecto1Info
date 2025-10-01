// Dirección del endpoint que devuelve JSON
const url = "http://172.16.189.250"

// Función que hace la petición GET y procesa el JSON
// Función para pedir datos al servidor
async function fetchDatos() 
{
  const response = await fetch(url);

  if (!response.ok) 
  {
    throw new Error("Error en la respuesta: " + response.status);
  }

  return await response.json();
}

async function obtenerDatos() 
{
  try 
  {
    const datos = await fetchDatos();
    console.log("Datos recibidos:", datos);
    actualizarControles(datos);
  } 
  catch (error) 
  {
    console.error("Error al obtener datos:", error);
  }
}

function actualizarHumedad(valor) {
  document.getElementById("lblHumedad").textContent = valor + "%";
  document.getElementById("barraHumedad").value = valor;
}


function actualizarControles(datos) {
  let lblLuz = document.getElementById("lblLuz");
  let iconoLuz = document.getElementById("iconoLuz");
  let tempImage = document.getElementById("iconoTemperatura"); // si existe
  let lblTemp = document.getElementById("lblTemperatura");
  let lblPuerta = document.getElementById("lblPuerta");
  let iconoPuerta = document.getElementById("iconoPuerta");
  let lblMovimiento = document.getElementById("lblMovimiento");
  let iconoMovimiento = document.getElementById("iconoMovimiento");

  if (datos.luzEncendida) {
    lblLuz.textContent = "Encendida";
    iconoLuz.src = "Foco encendido.png";
  } else {
    lblLuz.textContent = "Apagada";
    iconoLuz.src = "Foco apagado.png";
  }

  lblTemp.textContent = datos.temperatura;
  if (datos.temperatura <= 30) {
    tempImage.src = "termometro27.png";
  } else if (datos.temperatura <= 35.0) {
    tempImage.src = "termometro33-34.png";
  } else if (datos.temperatura <= 40.0) {
    tempImage.src = "termometro38-39.png";
  }

  if (datos.puertaAbierta) {
    lblPuerta.textContent = "Abierta";
    iconoPuerta.src = "Puerta abierta.png";
  } else {
    lblPuerta.textContent = "Cerrada";
    iconoPuerta.src = "Puerta cerrada.png";
  }

  if (datos.movimiento) {
    lblMovimiento.textContent = "Se detectó movimiento";
    iconoMovimiento.src = "Sensor activo.png";
  } else {
    lblMovimiento.textContent = "No se detectó movimiento";
    iconoMovimiento.src = "Sensor inactivo.png";
  }

  actualizarHumedad(datos.humedad);
}


// Llamada inicial
obtenerDatos();

// Repetir cada 3 segundos (3000 ms)
setInterval(obtenerDatos, 3000);

async function accion(comando)
{
	await fetch(url + "/" + comando);
}