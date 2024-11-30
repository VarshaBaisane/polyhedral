
# import json
# from channels.generic.websocket import AsyncWebsocketConsumer

# class EditorConsumer(AsyncWebsocketConsumer):
#     async def connect(self):
#         # Extract document ID from the URL
#         self.doc_id = self.scope['url_route']['kwargs']['doc_id']
#         self.group_name = f'document_{self.doc_id}'

#         # Join the group
#         await self.channel_layer.group_add(
#             self.group_name,
#             self.channel_name
#         )
#         await self.accept()

#     async def disconnect(self, close_code):
#         # Leave the group
#         await self.channel_layer.group_discard(
#             self.group_name,
#             self.channel_name
#         )

#     async def receive(self, text_data):
#         # Broadcast received changes to the group
#         data = json.loads(text_data)
#         await self.channel_layer.group_send(
#             self.group_name,
#             {
#                 'type': 'document_update',
#                 'document_content': data['document_content']
#             }
#         )

#     async def document_update(self, event):
#         # Send the updated document content to WebSocket
#         await self.send(text_data=json.dumps({
#             'document_content': event['document_content']
#         }))



<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Collaborative Text Editor</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      display: flex;
      flex-direction: column;
      align-items: center;
    }
    .toolbar {
      display: flex;
      gap: 10px;
      margin-bottom: 10px;
    }
    .toolbar .icon {
      padding: 8px;
      cursor: pointer;
      border: 1px solid #ccc;
      background-color: #f9f9f9;
      border-radius: 4px;
      display: flex;
      align-items: center;
      justify-content: center;
      font-size: 18px;
      font-weight: bold;
    }
    .toolbar select {
      padding: 5px;
    }
    .editor-container {
      width: 80%;
      border: 1px solid #ccc;
      padding: 20px;
      background-color: #fff;
      min-height: 300px;
      position: relative;
      overflow-y: auto;
      font-size: 16px;
    }
  </style>
</head>
<body>

  <!-- Toolbar for text formatting -->
  <div class="toolbar">
    <div class="icon" onclick="applyFormatting('bold')">B</div>
    <div class="icon" onclick="applyFormatting('italic')">I</div>
    <div class="icon" onclick="applyFormatting('underline')">U</div>
    <select onchange="changeFontSize(this.value)">
      <option value="">Font Size</option>
      <option value="12px">12px</option>
      <option value="14px">14px</option>
      <option value="16px">16px</option>
      <option value="18px">18px</option>
      <option value="24px">24px</option>
    </select>
    <select onchange="changeFontStyle(this.value)">
      <option value="">Font Style</option>
      <option value="Arial">Arial</option>
      <option value="Times New Roman">Times New Roman</option>
      <option value="Courier New">Courier New</option>
      <option value="Verdana">Verdana</option>
    </select>
    <input type="color" onchange="changeFontColor(this.value)" title="Font Color">
    <div class="icon" onclick="saveDocument()">💾</div>
    <div class="icon" onclick="exportDocument()">⤓</div>
    <input type="file" onchange="importDocument(event)" title="Import Document">
  </div>

  <!-- Editor Container -->
  <div class="editor-container" contenteditable="true" id="textEditor">
    <!-- Editable text area where users can type -->
  </div>

  <script>
    const docId = 'sample-doc';  // Unique document ID (could be dynamic)
    const socket = new WebSocket('ws://' + window.location.host + '/ws/editor/' + docId + '/');

    socket.onopen = function() {
      console.log('WebSocket connection established.');
    };

    socket.onmessage = function(event) {
      const data = JSON.parse(event.data);
      const editor = document.getElementById('textEditor');

      // Prevent overwriting by only updating if the content has changed
      if (editor.innerHTML !== data.document_content) {
        editor.innerHTML = data.document_content;
      }
    };

    // Send content when the user types
    document.getElementById('textEditor').addEventListener('input', function() {
      const content = this.innerHTML;
      socket.send(JSON.stringify({ 'document_content': content }));
    });

    socket.onclose = function() {
      console.log("WebSocket connection closed.");
    };

    socket.onerror = function(error) {
      console.error("WebSocket error observed:", error);
    };

    // Text formatting functions (unchanged from your original code)
    function applyFormatting(command) {
      document.execCommand(command, false, null);
    }

    function changeFontSize(size) {
      document.execCommand('fontSize', false, '7');
      const fontElements = document.getElementsByTagName('font');
      for (let fontElement of fontElements) {
        if (fontElement.size === "7") {
          fontElement.removeAttribute('size');
          fontElement.style.fontSize = size;
        }
      }
    }

    function changeFontStyle(style) {
      document.execCommand('fontName', false, style);
    }

    function changeFontColor(color) {
      document.execCommand('foreColor', false, color);
    }

    function saveDocument() {
      const content = document.getElementById('textEditor').innerHTML;
      localStorage.setItem('documentContent', content);
      alert("Document saved locally!");
    }

    function exportDocument() {
      const content = document.getElementById('textEditor').innerText;
      const blob = new Blob([content], { type: 'text/plain' });
      const link = document.createElement('a');
      link.href = URL.createObjectURL(blob);
      link.download = 'document.txt';
      link.click();
    }

    function importDocument(event) {
      const file = event.target.files[0];
      if (file) {
        const reader = new FileReader();
        reader.onload = function(e) {
          document.getElementById('textEditor').innerHTML = e.target.result;
        };
        reader.readAsText(file);
      }
    }
  </script>
</body>
</html>
