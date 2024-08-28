<?php

// $uploadDir = "./uploads/";
$uploadDir = getenv('UPLOAD_DIR');
echo $uploadDir;

if ($uploadDir === false)
{
	echo "<h2>Error: UPLOAD_DIR environment variable is not set.</h2>";
	exit();
}

echo "<p>Upload directory: " . htmlspecialchars($uploadDir) . "</p>";

if (!is_dir($uploadDir))
{
	if (!mkdir($uploadDir, 0777, true))
	{
		echo "<h2>Error: Failed to create directory '$uploadDir'.</h2>";
		exit();
	}
	else
	{
		echo "<p>Directory '$uploadDir' created successfully.</p>";
	}
}

if ($_SERVER["REQUEST_METHOD"] == "POST" && isset($_FILES["photo"]))
{
	$file = $_FILES["photo"];

	$fileName = basename($file["name"]);
	$targetFilePath = $uploadDir . $fileName;
	$fileType = strtolower(pathinfo($targetFilePath, PATHINFO_EXTENSION));
	$allowedTypes = array('jpg', 'jpeg', 'png', 'gif', 'webp');

	// Check if the file is an image and if it is an allowed type
	$check = getimagesize($file["tmp_name"]);
	if ($check !== false && in_array($fileType, $allowedTypes))
	{
		// Move the uploaded file to the destination directory
		if (move_uploaded_file($file["tmp_name"], $targetFilePath))
		{
			echo "<h2>The file has been uploaded successfully.</h2>";
			echo "<p>File name: " . htmlspecialchars($fileName) . "</p>";
			echo "<p>File type: " . htmlspecialchars($fileType) . "</p>";
			echo "<p>File path: " . htmlspecialchars($targetFilePath) . "</p>";
			echo '<p><img src="' . htmlspecialchars($targetFilePath) . '" alt="Uploaded Image" style="max-width:100%;"></p>';
		}
		else
			echo "<h2>Error moving the uploaded file.</h2>";
	}
	else
		echo "<h2>Unsupported Media Type</h2>";
}
else
{
	echo "<h2>Unauthorized access or file not sent.</h2>";
	echo "<p>Method: " . htmlspecialchars($_SERVER["REQUEST_METHOD"]) . "</p>";
	echo "<p>Files: " . htmlspecialchars(print_r($_FILES, true)) . "</p>";
}
?>
