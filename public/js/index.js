/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   index.js                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelomet <sbelomet@42lausanne.ch>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 10:09:19 by lgosselk          #+#    #+#             */
/*   Updated: 2024/09/19 15:47:19 by sbelomet         ###   ########.fr       */
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
				console.log("chill.txtx deleted successfully");
			} else {
				console.error("Failed to delete chill.txt");
			}
		})
		.catch(error => {
			console.error("An error occurred while deleting chill.txt:", error);
		});
});

document.body.appendChild(button);