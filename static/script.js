function prepararDrink(url) {
	fetch(url)
		.then(res => res.text())
		.then(msg => {
			if (msg.includes("finalizado")) {
				mostrarPopup("🍹 Drink pronto!", msg);
			} else {
				mostrarPopup("⏳ Aguarde", msg);
			}
		})
		.catch(() => {
			mostrarPopup("❌ Erro", "Falha ao comunicar com o servidor");
		});
}

function mostrarPopup(titulo, mensagem) {
	document.getElementById("popup-title").innerText = titulo;
	document.getElementById("popup-msg").innerText = mensagem;
	document.getElementById("popup").style.display = "flex";
}

function fecharPopup() {
	document.getElementById("popup").style.display = "none";
}
