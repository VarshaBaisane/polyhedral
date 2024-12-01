"""project2 URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/4.1/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  path('', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  path('', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.urls import include, path
    2. Add a URL to urlpatterns:  path('blog/', include('blog.urls'))
"""

# project2/urls.py




# project2/urls.py
''' from django.contrib import admin
from django.urls import path, include

urlpatterns = [
    path('admin/', admin.site.urls),
    path('editor/', include('editor.urls')),  # Include the URLs for the editor app
    path('', include('editor.urls')),  # Redirect root URL to the editor app
    path('auth/', include('django.contrib.auth.urls')),  # Add built-in auth views
]
'''

from django.urls import path, include
from django.http import HttpResponse

# Temporary root view
def home_view(request):
    return HttpResponse("<h1>Welcome to the Project</h1><p><a href='/editor/login/'>Login Here</a></p>")

urlpatterns = [
    path('', home_view, name='home'),  # Root URL
    path('editor/', include('editor.urls')),  # Includes editor app URLs
]
