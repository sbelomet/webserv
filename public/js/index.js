/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   index.js                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelomet <sbelomet@42lausanne.ch>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 10:09:19 by lgosselk          #+#    #+#             */
/*   Updated: 2024/09/26 12:52:15 by sbelomet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

let	text;
let	header;
let thing;
thing = document.getElementById("thing");

header = document.getElementById("omega");
text = document.createTextNode("🤠🤠🤠 WEBSERV 🤠🤠🤠");
header.appendChild(text);

const button = document.createElement("button");
button.textContent = "Delete chill.txt";
button.addEventListener("click", () => {
	fetch("/red/chill.txt", {
		method: "DELETE"
	})
		.then(response => {
			if (response.ok) {
				console.log("chill.txt deleted successfully");
			} else {
				console.error("Failed to delete chill.txt");
				window.alert("Couldn't find file to delete");
			}
		})
		.catch(error => {
			console.error("An error occurred while deleting chill.txt:", error);
		});
});

thing.appendChild(button);