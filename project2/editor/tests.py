# editor/tests.py
# editor/tests.py
from django.test import TestCase
from django.contrib.auth.models import User

class UserAuthenticationTests(TestCase):
    def setUp(self):
        # Create a test user
        self.user = User.objects.create_user(username='testuser', password='testpassword')

    def test_login_valid_user(self):
        # Test valid login
        response = self.client.post('/editor/login/', {'username': 'testuser', 'password': 'testpassword'})
        self.assertEqual(response.status_code, 200)  # Expecting 200 OK for valid login
        self.assertContains(response, "Welcome, testuser")

    def test_login_invalid_user(self):
        # Test invalid login
        response = self.client.post('/editor/login/', {'username': 'wronguser', 'password': 'wrongpassword'})
        
        # Check for the 401 Unauthorized status code
        self.assertEqual(response.status_code, 401)  # Expecting 401 Unauthorized for invalid login
        
        # Manually check the response content for "Invalid credentials" message
        self.assertIn("Invalid credentials", response.content.decode())  # Check the error message content


from channels.testing import WebsocketCommunicator
from django.test import TransactionTestCase
from project2.asgi import application  # Replace 'project2' with your project name

class WebSocketConnectivityTests(TransactionTestCase):
    async def test_websocket_connectivity(self):
        # Initialize the WebSocketCommunicator
        communicator = WebsocketCommunicator(application, "/ws/editor/test-doc/")
        
        # Attempt to connect
        connected, _ = await communicator.connect()
        self.assertTrue(connected, "WebSocket connection failed.")

        # Send a message
        test_message = {"type": "message", "content": "Hello"}
        await communicator.send_json_to(test_message)
        
        # Receive the response
        response = await communicator.receive_json_from()
        self.assertEqual(response, test_message, "The received message does not match the sent message.")

        # Disconnect
        await communicator.disconnect()

