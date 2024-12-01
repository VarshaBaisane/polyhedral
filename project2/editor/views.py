'''
from django.contrib.auth import login, authenticate
from django.contrib.auth.models import User
from django.shortcuts import render, redirect
from django import forms

class EmailAuthenticationForm(forms.Form):
    email = forms.EmailField()
    password = forms.CharField(widget=forms.PasswordInput)

def login_view(request):
    form = EmailAuthenticationForm(request.POST or None)
    if request.method == "POST" and form.is_valid():
        email = form.cleaned_data["email"]
        password = form.cleaned_data["password"]
        
        # Authenticate the user by email
        try:
            user = User.objects.get(email=email)  # Check if the email exists
            authenticated_user = authenticate(username=user.username, password=password)
            if authenticated_user is not None:
                login(request, authenticated_user)  # Log the user in
                return redirect("/")  # Redirect to the homepage or dashboard
            else:
                form.add_error(None, "Invalid email or password.")
        except User.DoesNotExist:
            form.add_error("email", "No account found with this email.")
    
    return render(request, "editor/login.html", {"form": form})
'''

from django.contrib.auth import authenticate, login
from django.shortcuts import render, redirect
from django.contrib.auth.forms import AuthenticationForm

def login_view(request):
    if request.method == 'POST':
        form = AuthenticationForm(request, data=request.POST)
        if form.is_valid():
            user = form.get_user()
            login(request, user)
            return redirect('/')
        else:
            return render(request, 'login.html', {'form': form})
    else:
        form = AuthenticationForm()
    return render(request, 'login.html', {'form': form})
