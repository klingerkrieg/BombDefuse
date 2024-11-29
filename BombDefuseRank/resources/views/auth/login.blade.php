@extends('layouts.app')

@section('title', 'Login')

@section('content')

<main id="guest">
    <div class="container">
        <div class="d-flex flex-column gap-1 align-items-center">
            <img class="logo" src="{{ asset("img/logo.png") }}" alt="Logo">
            <h1 class="text-center fw-bold">Login</h1>
            <h5 class="text-center">Insira suas credenciais de acesso.</h5>
            <a class="btn btn-dark" href="{{ url()->previous() ?: route('home') }}">
                <i class="bi bi-arrow-left"></i>
                Voltar
            </a>
        </div>
        <form style="max-width: 500px; margin: 0 auto;" class="d-block my-5" action="{{ route('login') }}" method="POST">
            @csrf

            <div class="form-group">
                <input class="@error('username') is-invalid @enderror form-control form-control-lg" type="text" name="username" id="username" value="{{ old('username') }}" placeholder="Username">
    
                @error('username')
                    <small class="text-danger fw-bold">{{ $message }}</small>
                @enderror
            </div>

            <div class="form-group my-4">
                <input class="@error('password') is-invalid @enderror form-control form-control-lg" type="password" name="password" id="password" placeholder="Senha">
    
                @error('password')
                    <small class="text-danger fw-bold">{{ $message }}</small>
                @enderror
            </div>

            <div class="form-group">
                <button class="w-100 btn btn-lg btn-primary shadow d-flex align-items-center justify-content-center gap-1" type="submit">
                    <svg xmlns="http://www.w3.org/2000/svg" height="32px" viewBox="0 -960 960 960" width="32px" fill="currentColor"><path d="M480.67-120v-66.67h292.66v-586.66H480.67V-840h292.66q27 0 46.84 19.83Q840-800.33 840-773.33v586.66q0 27-19.83 46.84Q800.33-120 773.33-120H480.67Zm-63.34-176.67-47-48 102-102H120v-66.66h351l-102-102 47-48 184 184-182.67 182.66Z"/></svg>
                    Entrar
                </button>
            </div>
        </form>
    </div>
</main>

@endsection