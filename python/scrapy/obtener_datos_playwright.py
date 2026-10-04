import asyncio
import json
from urllib.parse import urljoin

from playwright.async_api import async_playwright


URL_RESULTADOS = (
    "https://votoinformado.jne.gob.pe/candidatos/resultados"
    "?departamento=Cusco&depCode=07&provincia=Cusco&provCode=01"
)
URL_BASE = "https://votoinformado.jne.gob.pe"
ARCHIVO_SALIDA = "consejeros_cusco_2026.json"


def extraer_seccion(texto: str, marcador_inicio: str, marcador_fin: str) -> str:
    inicio = texto.find(marcador_inicio)
    if inicio == -1:
        raise ValueError(f"No se encontró la sección: {marcador_inicio}")

    inicio += len(marcador_inicio)
    fin = texto.find(marcador_fin, inicio)
    if fin == -1:
        raise ValueError(f"No se encontró el final de la sección: {marcador_fin}")

    return texto[inicio:fin].strip()


async def extraer_hojas_vida() -> None:
    async with async_playwright() as playwright:
        browser = await playwright.chromium.launch(headless=False)
        try:
            page = await browser.new_page()
            await page.goto(URL_RESULTADOS, wait_until="domcontentloaded")

            boton_cargo = page.locator("button[aria-expanded]").filter(
                has_text="Consejero Regional"
            )
            await boton_cargo.first.wait_for(state="visible", timeout=15000)
            cantidad_secciones = await boton_cargo.count()
            if cantidad_secciones != 1:
                raise RuntimeError(
                    "Se esperaba una sección Consejero Regional, "
                    f"pero se encontraron {cantidad_secciones}."
                )
            if await boton_cargo.get_attribute("aria-expanded") != "true":
                await boton_cargo.click()

            seccion_cargo = boton_cargo.locator("xpath=..")
            botones_organizaciones = seccion_cargo.locator(
                'button[title^="Ver candidatos de "]'
            )
            total_organizaciones = await botones_organizaciones.count()
            if total_organizaciones == 0:
                raise RuntimeError(
                    "No se encontraron organizaciones para Consejero Regional."
                )

            candidatos_por_url: dict[str, dict[str, str]] = {}
            for indice in range(total_organizaciones):
                boton_organizacion = botones_organizaciones.nth(indice)
                titulo = await boton_organizacion.get_attribute("title")
                if not titulo:
                    raise RuntimeError("Una organización no tiene título.")

                organizacion = titulo.replace("Ver candidatos de ", "", 1)
                await boton_organizacion.click()

                dialogo = page.locator('[role="dialog"]')
                await dialogo.wait_for(state="visible")
                enlaces_candidatos = dialogo.locator(
                    'a[aria-label^="Ver hoja de vida de "]'
                    '[href^="/candidatos/hoja-vida/"]'
                )
                await enlaces_candidatos.first.wait_for(
                    state="visible", timeout=15000
                )
                cantidad_candidatos = await enlaces_candidatos.count()
                if cantidad_candidatos == 0:
                    raise RuntimeError(
                        f"No se encontraron candidatos en {organizacion}."
                    )

                for indice_candidato in range(cantidad_candidatos):
                    enlace = enlaces_candidatos.nth(indice_candidato)
                    href = await enlace.get_attribute("href")
                    aria_label = await enlace.get_attribute("aria-label")
                    if not href or not aria_label:
                        raise RuntimeError(
                            f"Enlace de hoja de vida incompleto en {organizacion}."
                        )

                    url_perfil = urljoin(URL_BASE, href)
                    candidatos_por_url[url_perfil] = {
                        "nombre": aria_label.replace(
                            "Ver hoja de vida de ", "", 1
                        ),
                        "agrupacion": organizacion,
                    }

                await dialogo.get_by_role("button", name="Cerrar").click()
                await dialogo.wait_for(state="hidden")

            if not candidatos_por_url:
                raise RuntimeError(
                    "No se encontraron candidatos en Consejero Regional."
                )

            print(
                f"Organizaciones de Consejero Regional: {total_organizaciones}; "
                f"candidatos encontrados: {len(candidatos_por_url)}."
            )

            datos_completos = []
            for url_perfil, candidato in candidatos_por_url.items():
                await page.goto(url_perfil, wait_until="domcontentloaded")
                await page.locator("h1").first.wait_for()
                nombre = (await page.locator("h1").first.inner_text()).strip()
                texto_perfil = await page.locator("body").inner_text()

                datos_completos.append(
                    {
                        **candidato,
                        "nombre": nombre,
                        "cargo": "Consejero Regional",
                        "estudios": extraer_seccion(
                            texto_perfil, "EDUCACIÓN BÁSICA", "EXPERIENCIA LABORAL"
                        ),
                        "experiencia": extraer_seccion(
                            texto_perfil,
                            "EXPERIENCIA LABORAL",
                            "TRAYECTORIA PARTIDARIA",
                        ),
                        "sentencias": extraer_seccion(
                            texto_perfil,
                            "DECLARACIÓN DE SENTENCIAS FIRMES",
                            "INGRESO DE BIENES Y RENTAS",
                        ),
                        "url_hoja_vida": url_perfil,
                    }
                )

            with open(ARCHIVO_SALIDA, "w", encoding="utf-8") as archivo:
                json.dump(datos_completos, archivo, ensure_ascii=False, indent=4)

            print(f"Datos guardados en {ARCHIVO_SALIDA}.")
        finally:
            await browser.close()


asyncio.run(extraer_hojas_vida())
