@extends('layouts.auth')

@if ($equipe->id)
    @section('title', 'Editar Equipe')
@else
    @section('title', 'Adicionar Equipe na Fila')
@endif

<style>

    red{
        color:red;
    }
    
</style>

@section('content')

    <main>
        <div class="container">
            <div class="row justify-content-center">
                <div class="col-md-8">
                    <div class="card">
                        @if($equipe->id)
                            <div class="card-header">{{ $equipe->nome }}</div>
                        @else
                            <div class="card-header">{{ __('Adicionar Equipe na Fila') }}</div>
                        @endif
        
                        <div class="card-body">
        
                            @if ($equipe->id)
                                <form id="form" action="{{route('fila.update', $equipe)}}" method="POST">
                                @method('PUT')
                                @csrf
                            @else
                                <form id="form" action="{{route('fila.store')}}" method="POST">
                                 @csrf
                            @endif
        
                                <div class='row mb-3'>
                                    <label class='col-md'>Nome da Equipe <red>*</red>
                                        <input name="nome" class="form-control @error('nome') is-invalid @enderror" type="text" value="{{$equipe->nome}}">
        
                                        @error('nome')
                                            <small class="text-danger fw-bold">{{ $message }}</small>
                                        @enderror
                                    </label>
                                </div>

                                <div class='row mb-3'>
                                    <label class='col-md'>Whatsapp do Líder <red>*</red>
                                        <input name="whatsapp" class="form-control @error('whatsapp') is-invalid @enderror" type="text" value="{{ $equipe->whatsapp }}" maxlength="15" onkeyup="handlePhone(event)">
        
                                        @error('whatsapp')
                                            <small class="text-danger fw-bold">{{ $message }}</small>
                                        @enderror
                                    </label>
                                </div>

                                <div class="d-flex justify-content-end gap-2">
                                    <a class="btn btn-secondary" href="{{ route('fila') }}">Voltar</a>
                                    <button class="btn btn-primary" type="submit">
                                        {{ $equipe->id ? 'Salvar' : 'Adicionar' }}
                                    </button>
                                </div>
        
                            </form>

                            {{-- Excluir Equipe --}}
                            @if($equipe->id)
                                <form action="{{ route('fila.destroy', $equipe->id) }}" method="POST">
                                    @csrf
                                    @method('DELETE')
                                    <button class="btn btn-danger" type="submit">Excluir</button>
                                </form>
                            @endif
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </main>

    <script src="{{ asset('js/script.js') }}"></script>
@endsection
