import os
import cgi
import sys
import html

# Ensure the current directory is set correctly
os.chdir(os.path.dirname(sys.argv[0]))

# Configuration
script_dir = os.path.dirname(os.path.abspath(__file__))
upload_dir = os.getenv('UPLOAD_DIR', os.path.join(script_dir, 'var', 'upload'))

# Check if upload directory exists, create it if not
if not os.path.exists(upload_dir):
	try:
		os.makedirs(upload_dir)
		print(f"<p>Directory '{html.escape(upload_dir)}' created successfully.</p>")
	except OSError as e:
		print(f"<h2>Error: Failed to create directory '{html.escape(upload_dir)}': {html.escape(str(e))}</h2>")
		sys.exit()

# Handle file upload
form = cgi.FieldStorage()
if 'photo' in form:
	fileitem = form['photo']

	# Ensure it's a file upload
	if fileitem.file:
		# Construct target file path relative to script directory
		filename = os.path.basename(fileitem.filename)
		target_filepath = os.path.join(upload_dir, filename)

		# Save the uploaded file
		with open(target_filepath, 'wb') as f:
			f.write(fileitem.file.read())

		print("<h2>The file has been uploaded successfully.</h2>")
		print(f"<p>File name: {html.escape(filename)}</p>")
		print(f"<p>File type: {html.escape(fileitem.type)}</p>")
		print(f"<p>File path: {html.escape(target_filepath)}</p>")
		print(f'<p><img src="{html.escape(target_filepath)}" alt="Uploaded Image" style="max-width:100%;"></p>')
	else:
		print("<h2>Error: No file uploaded or file upload error.</h2>")
else:
	print("<h2>Unauthorized access or file not sent.</h2>")
	print(f"<p>Method: {html.escape(os.environ['REQUEST_METHOD'])}</p>")
	print(f"<p>Files: {html.escape(str(form))}</p>")


