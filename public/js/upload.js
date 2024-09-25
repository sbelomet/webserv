/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   upload.js                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgosselk <lgosselk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 11:04:11 by sbelomet          #+#    #+#             */
/*   Updated: 2024/09/25 15:40:16 by lgosselk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

const MAX_FILE_SIZE = 150000; // 150 KB

document.getElementById('upload-form').addEventListener('submit', function (e) {
	e.preventDefault();
	uploadFile();
  });
  
  async function uploadFile() {
	const fileInput = document.getElementById('file-upload');
	const file = fileInput.files[0];
  
	if (!file) {
	  alert('Please select a file dumbass 😀😀😀');
	  return;
	}

	if (file.size > MAX_FILE_SIZE) {
		alert('File (' + file.size + ' bytes) is too big! Max size is ' + MAX_FILE_SIZE + ' bytes');
		return;
	}

	const formData = new FormData();
	formData.append('file', file);

	try {
	  const response = await fetch('../../cgi-bin/upload.py', {
		method: 'POST',
		body: formData,
	  });
  
	  if (!response.ok) {
		throw new Error('Network response was not ok');
	  }

	  const data = await response.json();
	  console.log('File uploaded successfully:', data);
	}
	catch (error) {
	  console.error('Error uploading file:', error);
	}
  }
  