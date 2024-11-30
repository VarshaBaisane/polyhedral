
# editor/urls.py

# editor/urls.py
from django.urls import path
from . import views  # Import views from the current app

urlpatterns = [
    path('', views.editor_view, name='editor'),  # Route for the editor home page
]


