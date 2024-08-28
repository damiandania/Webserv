<?php

if ($_SERVER["REQUEST_METHOD"] == "POST" || $_SERVER["REQUEST_METHOD"] == "GET")
{
	$params = ($_SERVER["REQUEST_METHOD"] == "POST") ? $_POST : $_GET;
	$name = isset($params["name"]) ? htmlspecialchars(trim($params["name"])) : "";
	$lastname = isset($params["lastname"]) ? htmlspecialchars(trim($params["lastname"])) : "";
	$email = isset($params["email"]) ? htmlspecialchars(trim($params["email"])) : "";
	$action = isset($params["action"]) ? $params["action"] : "";
	$filePath = "./contacts.txt";

	if (!empty($name) && !empty($lastname) && filter_var($email, FILTER_VALIDATE_EMAIL))
	{
		$contactLine = $name . " " . $lastname . " - " . $email . "\n";
		$file = fopen($filePath, "a");
		if ($file)
		{
			fwrite($file, $contactLine);
			fclose($file);
			generateContactsHtml($filePath);
			$body = createContactOkPage($name, $lastname, $email, $filePath);
		}
		else
		{
			$body = "<h2>Error opening the file.</h2>";
		}
	}
	elseif ($action == "delete")
	{
		$nameToDelete = isset($params["nameToDelete"]) ? htmlspecialchars(trim($params["nameToDelete"])) : "";
		$emailToDelete = isset($params["emailToDelete"]) ? htmlspecialchars(trim($params["emailToDelete"])) : "";

		if (!empty($nameToDelete) && !empty($emailToDelete))
		{
			$contacts = file($filePath);
			$newContacts = array();

			foreach ($contacts as $contact)
			{
				$contactData = explode(" - ", trim($contact));
				$existingName = $contactData[0];
				$existingEmail = $contactData[1];

				if ($existingName !== $nameToDelete || $existingEmail !== $emailToDelete)
					$newContacts[] = $contact;
			}
			file_put_contents($filePath, implode("", $newContacts));
			generateContactsHtml($filePath);
			$body = createDeleteOkPage($nameToDelete, $emailToDelete);
			$body = "<h2>Contact deleted successfully.</h2>";
		}
	}
	else
	{
		$body = "<h2>Invalid information submitted.</h2>";
	}
}
else
{
	$body = "<h2>Unauthorized access.</h2>";
}

echo $body;

// Fonction pour générer le header commun aux pages HTML
function generateHeader($pageTitle)
{
	$htmlContent = <<<HTML
<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<title>$pageTitle</title>
	<style>
		body {
			font-family: Arial, sans-serif;
			background-color: #9;
			color: #333;
			margin: 0;
			padding: 0;
			display: flex;
			flex-direction: column;
			min-height: 100vh;
		}
		header, footer {
			text-align: right;
			padding: 10px 20px;
			background-color: #333;
			color: #fff;
		}
		header em, footer em {
			font-size: 0.9em;
		}
		main {
			text-align: center;
			padding: 20px;
			flex: 1;
		}
		h1 {
			color: #333;
			font-size: 2.5em;
			margin-top: 0;
		}
		h4 {
			color: #666;
			font-size: 1.2em;
		}
		img {
			margin-top: 20px;
		}
		.links {
			margin-top: 20px;
		}
		.links p {
			margin: 10px 0;
			font-size: 1.1em;
		}
		.links a {
			color: #007BFF;
			text-decoration: none;
		}
		.links a:hover {
			text-decoration: underline;
		}
	</style>
</head>
<body>
	<header>
		<em>42 Paris - Created by arincon and ddania-c 2024</em>
	</header>
	<main>
		<a href="/index.html">
			<img src="https://logowik.com/content/uploads/images/423918.logowik.com.webp" alt="42_logo" width="200" height="150" />
		</a>
		<h1>Webserv Project</h1>
HTML;

	return $htmlContent;
}

function createContactOkPage($name, $lastname, $email)
{
	$contactPath = "../forms/contacts.html";
	$headerContent = generateHeader("Webserv Project - Contact Submitted");
	$bodyContent = "<h2>Form submitted successfully!</h2>";
	$bodyContent .= "<p>Name: " . $name . "</p>";
	$bodyContent .= "<p>Last Name: " . $lastname . "</p>";
	$bodyContent .= "<p>Email: " . $email . "</p>";
	$bodyContent .= "<p><a href='" . $contactPath . "' >View Contacts</a></p>";
	$htmlContent = $headerContent . "\n" . $bodyContent . "\n</main>\n</body>\n</html>";
	$outputPath = "../forms/contact_ok.html";
	file_put_contents($outputPath, $htmlContent);
	return $bodyContent;
}

function createDeleteOkPage($name, $email)
{
	$contactPath = "../forms/contacts.html";
	$headerContent = generateHeader("Webserv Project - Contact Deleted");
	$bodyContent = "<h2>Contact deleted successfully!</h2>";
	$bodyContent .= "<p>Name: " . $name . "</p>";
	$bodyContent .= "<p>Email: " . $email . "</p>";
	$bodyContent .= "<p><a href='" . $contactPath . "'>View Contacts</a></p>";
	$htmlContent = $headerContent . "\n" . $bodyContent . "\n</main>\n</body>\n</html>";
	$outputPath = "../forms/delete_ok.html";
	file_put_contents($outputPath, $htmlContent);
	return $bodyContent;
}

function generateContactsHtml($filePath)
{
	$headerContent = generateHeader("Webserv Project - Contacts");
	$contactsContent = "";
	if (file_exists($filePath) && filesize($filePath) > 0)
	{
		$contacts = file($filePath);
		if (empty($contacts))
		{
			$contactsContent = "<p>No contacts found.</p>";
			$contactsContent .= "<p>Return to the <a href=\"/index.html\">home page</a>.</p>";
		}
		else
		{
			foreach ($contacts as $contact)
			{
				$contactData = explode(" - ", trim($contact));
				$contactName = htmlspecialchars($contactData[0]);
				$contactEmail = htmlspecialchars($contactData[1]);
				$contactsContent .= "<form action='submit.php' method='POST'>";
				$contactsContent .= "<p id='contact_{$contactName}_{$contactEmail}'>" . $contactName . " - " . $contactEmail . " ";
				$contactsContent .= "<input type='hidden' name='action' value='delete'>";
				$contactsContent .= "<input type='hidden' name='nameToDelete' value='{$contactName}'>";
				$contactsContent .= "<input type='hidden' name='emailToDelete' value='{$contactEmail}'>";
				$contactsContent .= "<button type='submit'>Delete</button></p>";
				$contactsContent .= "</form>";
			}
		}
	}
	else
	{
		$contactsContent = "<p>No contacts found.</p>";
		$contactsContent .= "<p>Return to the <a href=\"/index.html\">home page</a>.</p>";
	}

	$outputContent = $headerContent . "\n" . $contactsContent . "\n</main>\n</body>\n</html>";
	$outputPath = "../forms/contacts.html";
	$outputFile = fopen($outputPath, "w");
	if ($outputFile)
	{
		fwrite($outputFile, $outputContent);
		fclose($outputFile);
	}
	else
	{
		die("Error creating output file.");
	}
	file_put_contents($outputPath, $outputContent);
}


?>
